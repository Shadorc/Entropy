#include "Precompiled.h"

CollisionCallback Dispatch[(int) EntityType::COUNT][(int) EntityType::COUNT] =
{
  {
	CircleToCircle, CircleToPolygon
  },
  {
	PolygonToCircle, PolygonToPolygon
  },
};

Collision Solve(Entity* entityA, Entity* entityB)
{
	Collision collision(entityA, entityB);
	Dispatch[(int) entityA->GetType()][(int) entityB->GetType()](collision);
	return collision;
}

void CircleToCircle(Collision& collision)
{
	const entity::Circle* circleA = dynamic_cast<const entity::Circle*>(collision.entityA);
	const entity::Circle* circleB = dynamic_cast<const entity::Circle*>(collision.entityB);

	const Vector2& deltaPos = circleB->position - circleA->position;
	float deltaLenSq = deltaPos.LengthSq();
	float sumRadius = circleA->GetRadius() + circleB->GetRadius();

	if (deltaLenSq >= sumRadius * sumRadius)
	{
		return;
	}

	if (IsZero(deltaLenSq))
	{
		collision.penetration = circleA->GetRadius();
		collision.normal = Vector2(1, 0);
		collision.contacts.push_back(circleA->position);
	}
	else
	{
		float deltaLen = sqrtf(deltaLenSq);
		collision.penetration = sumRadius - deltaLen;
		collision.normal = deltaPos / deltaLen;
		collision.contacts.push_back(collision.normal * circleA->GetRadius() + circleA->position);
	}
}

void CircleToPolygon(Collision& collision)
{
	entity::Circle* circleA = dynamic_cast<entity::Circle*>(collision.entityA);
	entity::Polygon* polygonB = dynamic_cast<entity::Polygon*>(collision.entityB);

	// Transform circle center to Polygon model space
	const Vector2& center = polygonB->GetOrientationMatrix().Transpose() * (circleA->position - polygonB->position);

	// Find edge with minimum penetration
	// Exact concept as using support points in Polygon vs Polygon
	float separation = -FLT_MAX;
	size_t faceNormal = 0;
	for (size_t i = 0; i < polygonB->GetVertexCount(); ++i)
	{
		float s = polygonB->GetNormal(i).Dot(center - polygonB->GetVertex(i));

		if (s > circleA->GetRadius())
		{
			return;
		}

		if (s > separation)
		{
			separation = s;
			faceNormal = i;
		}
	}

	// Grab face's vertices
	Vector2 v1 = polygonB->GetVertex(faceNormal);
	size_t i2 = (faceNormal + 1) % polygonB->GetVertexCount();
	Vector2 v2 = polygonB->GetVertex(i2);

	// Check to see if center is within polygon
	if (IsZero(separation))
	{
		collision.normal = -(polygonB->GetOrientationMatrix() * polygonB->GetNormal(faceNormal));
		collision.contacts.push_back(collision.normal * circleA->GetRadius() + circleA->position);
		collision.penetration = circleA->GetRadius();
		return;
	}

	// Determine which voronoi region of the edge center of circle lies within
	float dot1 = (center - v1).Dot(v2 - v1);
	float dot2 = (center - v2).Dot(v1 - v2);
	collision.penetration = circleA->GetRadius() - separation;

	// Closest to v1
	if (dot1 <= 0.0f)
	{
		if (center.DistanceSq(v1) > circleA->GetRadius() * circleA->GetRadius())
		{
			return;
		}

		Vector2 n = v1 - center;
		n = polygonB->GetOrientationMatrix() * n;
		n.Normalize();
		collision.normal = n;
		v1 = polygonB->GetOrientationMatrix() * v1 + polygonB->position;
		collision.contacts.push_back(v1);
	}
	// Closest to v2
	else if (dot2 <= 0.0f)
	{
		if (center.DistanceSq(v2) > circleA->GetRadius() * circleA->GetRadius())
		{
			return;
		}

		Vector2 n = v2 - center;
		v2 = polygonB->GetOrientationMatrix() * v2 + polygonB->position;
		collision.contacts.push_back(v2);
		n = polygonB->GetOrientationMatrix() * n;
		n.Normalize();
		collision.normal = n;
	}
	// Closest to face
	else
	{
		Vector2 n = polygonB->GetNormal(faceNormal);
		if ((center - v1).Dot(n) > circleA->GetRadius())
		{
			return;
		}

		n = polygonB->GetOrientationMatrix() * n;
		collision.normal = -n;
		collision.contacts.push_back(collision.normal * circleA->GetRadius() + circleA->position);
	}
}

void PolygonToCircle(Collision& collision)
{
	std::swap(collision.entityA, collision.entityB);
	CircleToPolygon(collision);
	collision.normal = -collision.normal;
}

void PolygonToPolygon(Collision& collision)
{
	entity::Polygon* polygonA = dynamic_cast<entity::Polygon*>(collision.entityA);
	entity::Polygon* polygonB = dynamic_cast<entity::Polygon*>(collision.entityB);

	// Check for a separating axis with polygon polygon A's face planes
	size_t faceA;
	float penetrationA = FindAxisLeastPenetration(&faceA, polygonA, polygonB);
	if (penetrationA >= 0.0f)
	{
		return;
	}

	// Check for a separating axis with polygon B's face planes
	size_t faceB;
	float penetrationB = FindAxisLeastPenetration(&faceB, polygonB, polygonA);
	if (penetrationB >= 0.0f)
	{
		return;
	}

	size_t referenceIndex;
	bool flip; // Always point from a to b

	entity::Polygon* polyRef; // Reference
	entity::Polygon* polyInc; // Incident

	// Determine which shape contains reference face
	if (BiasGreaterThan(penetrationA, penetrationB))
	{
		polyRef = polygonA;
		polyInc = polygonB;
		referenceIndex = faceA;
		flip = false;
	}
	else
	{
		polyRef = polygonB;
		polyInc = polygonA;
		referenceIndex = faceB;
		flip = true;
	}

	// World space incident face
	Vector2 incidentFace[2];
	FindIncidentFace(incidentFace, polyRef, polyInc, referenceIndex);

	//        y
	//        ^  ->n       ^
	//      +---c ------posPlane--
	//  x < | i |\
    //      +---+ c-----negPlane--
	//             \       v
	//              r
	//
	//  r : reference face
	//  i : incident poly
	//  c : clipped point
	//  n : incident normal

	// Setup reference face vertices
	Vector2 v1 = polyRef->GetVertex(referenceIndex);
	referenceIndex = (referenceIndex + 1) % polyRef->GetVertexCount();
	Vector2 v2 = polyRef->GetVertex(referenceIndex);

	// Transform vertices to world space
	v1 = polyRef->GetOrientationMatrix() * v1 + polyRef->position;
	v2 = polyRef->GetOrientationMatrix() * v2 + polyRef->position;

	// Calculate reference face side normal in world space
	Vector2 sidePlaneNormal = (v2 - v1);
	sidePlaneNormal.Normalize();

	// Orthogonalize
	Vector2 refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

	// ax + by = c
	// c is distance from origin
	float refC = refFaceNormal.Dot(v1);
	float negSide = -sidePlaneNormal.Dot(v1);
	float posSide = sidePlaneNormal.Dot(v2);

	// Clip incident face to reference face side planes
	if (Clip(-sidePlaneNormal, negSide, incidentFace) < 2)
	{
		return; // Due to floating point error, possible to not have required points
	}

	if (Clip(sidePlaneNormal, posSide, incidentFace) < 2)
	{
		return; // Due to floating point error, possible to not have required points
	}

	// Flip
	collision.normal = flip ? -refFaceNormal : refFaceNormal;

	// Keep points behind reference face
	float separation = refFaceNormal.Dot(incidentFace[0]) - refC;
	if (separation <= 0.0f)
	{
		collision.contacts.push_back(incidentFace[0]);
		collision.penetration = -separation;
	}
	else
	{
		collision.penetration = 0;
	}

	separation = refFaceNormal.Dot(incidentFace[1]) - refC;
	if (separation <= 0.0f)
	{
		collision.contacts.push_back(incidentFace[1]);
		collision.penetration += -separation;

		// Average penetration
		collision.penetration /= (float) collision.contacts.size();
	}
}

float FindAxisLeastPenetration(size_t* faceIdx, entity::Polygon* polygonA, entity::Polygon* polygonB)
{
	float bestDistance = -FLT_MAX;
	size_t bestIdx = 0;

	for (size_t i = 0; i < polygonA->GetVertexCount(); ++i)
	{
		// Retrieve a face normal from polygon A
		Vector2 normal = polygonA->GetNormal(i);
		const Vector2& nw = polygonA->GetOrientationMatrix() * normal;

		// Transform face normal into polygon B's model space
		const Matrix22& orientationTransposedB = polygonB->GetOrientationMatrix().Transpose();
		normal = orientationTransposedB * nw;

		// Retrieve support point from polygon B along -n
		const Vector2& support = polygonB->GetSupport(-normal);

		// Retrieve vertex on face from polygon A, transform into polygon B's model space
		Vector2 vertex = polygonA->GetVertex(i);
		vertex = polygonA->GetOrientationMatrix() * vertex + polygonA->position;
		vertex -= polygonB->position;
		vertex = orientationTransposedB * vertex;

		// Compute penetration distance (in polygon B's model space)
		float penetrationDistance = normal.Dot(support - vertex);

		// Store greatest distance
		if (penetrationDistance > bestDistance)
		{
			bestDistance = penetrationDistance;
			bestIdx = i;
		}
	}

	*faceIdx = bestIdx;
	return bestDistance;
}

void FindIncidentFace(Vector2 incidentFace[], entity::Polygon* polyRef, entity::Polygon* polyInc, size_t idxRef)
{
	Vector2 referenceNormal = polyRef->GetNormal(idxRef);

	// Calculate normal in incident's frame of reference
	referenceNormal = polyRef->GetOrientationMatrix() * referenceNormal; // To world space
	referenceNormal = polyInc->GetOrientationMatrix().Transpose() * referenceNormal; // To incident's model space

	// Find most anti-normal face on incident polygon
	size_t incidentFaceIdx = 0;
	float minDot = FLT_MAX;
	for (size_t i = 0; i < polyInc->GetVertexCount(); ++i)
	{
		float dot = referenceNormal.Dot(polyInc->GetNormal(i));
		if (dot < minDot)
		{
			minDot = dot;
			incidentFaceIdx = i;
		}
	}

	// Assign face vertices for incidentFace
	incidentFace[0] = polyInc->GetOrientationMatrix() * polyInc->GetVertex(incidentFaceIdx) + polyInc->position;
	incidentFaceIdx = (incidentFaceIdx + 1) % polyInc->GetVertexCount();
	incidentFace[1] = polyInc->GetOrientationMatrix() * polyInc->GetVertex(incidentFaceIdx) + polyInc->position;
}

int Clip(Vector2 n, float c, Vector2 face[])
{
	int sp = 0;
	Vector2 out[2] = {
	  face[0],
	  face[1]
	};

	// Retrieve distances from each endpoint to the line
	// d = ax + by - c
	float d1 = n.Dot(face[0]) - c;
	float d2 = n.Dot(face[1]) - c;

	// If negative (behind plane) clip
	if (d1 <= 0.0f)
	{
		out[sp++] = face[0];
	}
	if (d2 <= 0.0f) 
	{
		out[sp++] = face[1];
	}

	// If the points are on different sides of the plane
	if (d1 * d2 < 0.0f) // less than to ignore -0.0f
	{
		// Push interesection point
		float alpha = d1 / (d1 - d2);
		out[sp] = face[0] + alpha * (face[1] - face[0]);
		++sp;
	}

	// Assign our new converted values
	face[0] = out[0];
	face[1] = out[1];

	ENTROPY_ASSERT(sp != 3, "");

	return sp;
}

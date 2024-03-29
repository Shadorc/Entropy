#include "Precompiled.h"

CollisionCallback Dispatch[static_cast<int>(EntityType::COUNT)][static_cast<int>(EntityType::COUNT)] =
{
  {
	CircleToCircle, CircleToPolygon
  },
  {
	PolygonToCircle, PolygonToPolygon
  },
};

void Solve(Collision& collisionOut)
{
	Dispatch[static_cast<int>(collisionOut.entityA->GetType())][static_cast<int>(collisionOut.entityB->GetType())](collisionOut);
}

void CircleToCircle(Collision& collision)
{
	const entity::Circle* circleA = dynamic_cast<const entity::Circle*>(collision.entityA);
	ENTROPY_ASSERT(circleA);
	const entity::Circle* circleB = dynamic_cast<const entity::Circle*>(collision.entityB);
	ENTROPY_ASSERT(circleB);

	const Vector2& deltaPos = circleB->GetPosition() - circleA->GetPosition();
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
		collision.contacts.push_back(circleA->GetPosition());
	}
	else
	{
		float deltaLen = sqrtf(deltaLenSq);
		collision.penetration = sumRadius - deltaLen;
		collision.normal = deltaPos / deltaLen;
		collision.contacts.push_back(collision.normal * circleA->GetRadius() + circleA->GetPosition());
	}
}

void CircleToPolygon(Collision& collision)
{
	entity::Circle* circleA = dynamic_cast<entity::Circle*>(collision.entityA);
	ENTROPY_ASSERT(circleA);
	entity::Polygon* polygonB = dynamic_cast<entity::Polygon*>(collision.entityB);
	ENTROPY_ASSERT(polygonB);

	// Transform circle center to Polygon model space
	const Vector2& center = polygonB->GetOrientationMatrix().Transpose() * (circleA->GetPosition() - polygonB->GetPosition());

	// Find edge with minimum penetration
	// Exact concept as using support points in Polygon vs Polygon
	float separationMax = -FLT_MAX;
	size_t faceIdx = 0;
	for (size_t i = 0; i < polygonB->GetVertexCount(); ++i)
	{
		float separation = polygonB->GetNormal(i).Dot(center - polygonB->GetVertex(i));

		if (separation > circleA->GetRadius())
		{
			return;
		}

		if (separation > separationMax)
		{
			separationMax = separation;
			faceIdx = i;
		}
	}

	// Check to see if center is within polygon
	if (IsZero(separationMax))
	{
		collision.normal = -(polygonB->GetOrientationMatrix() * polygonB->GetNormal(faceIdx));
		collision.contacts.push_back(collision.normal * circleA->GetRadius() + circleA->GetPosition());
		collision.penetration = circleA->GetRadius();
		return;
	}

	collision.penetration = circleA->GetRadius() - separationMax;

	// Grab face's vertices
	Vector2 vertex1 = polygonB->GetVertex(faceIdx);
	const size_t vertex2Idx = (faceIdx + 1) % polygonB->GetVertexCount();
	Vector2 vertex2 = polygonB->GetVertex(vertex2Idx);

	// Determine which voronoi region of the edge center of circle lies within
	float dot1 = (center - vertex1).Dot(vertex2 - vertex1);
	float dot2 = (center - vertex2).Dot(vertex1 - vertex2);

	// Closest to vertex1
	if (dot1 <= 0.0f)
	{
		if (center.DistanceSq(vertex1) > circleA->GetRadius() * circleA->GetRadius())
		{
			return;
		}

		Vector2 normal = polygonB->GetOrientationMatrix() * (vertex1 - center);
		normal.Normalize();
		collision.normal = normal;
		vertex1 = polygonB->GetOrientationMatrix() * vertex1 + polygonB->GetPosition();
		collision.contacts.push_back(vertex1);
	}
	// Closest to vertex2
	else if (dot2 <= 0.0f)
	{
		if (center.DistanceSq(vertex2) > circleA->GetRadius() * circleA->GetRadius())
		{
			return;
		}

		Vector2 normal = polygonB->GetOrientationMatrix() * (vertex2 - center);
		normal.Normalize();
		collision.normal = normal;
		vertex2 = polygonB->GetOrientationMatrix() * vertex2 + polygonB->GetPosition();
		collision.contacts.push_back(vertex2);
	}
	// Closest to face
	else
	{
		Vector2 normal = polygonB->GetNormal(faceIdx);
		if ((center - vertex1).Dot(normal) > circleA->GetRadius())
		{
			return;
		}

		normal = polygonB->GetOrientationMatrix() * normal;
		collision.normal = -normal;
		collision.contacts.push_back(collision.normal * circleA->GetRadius() + circleA->GetPosition());
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
	ENTROPY_ASSERT(polygonA);
	entity::Polygon* polygonB = dynamic_cast<entity::Polygon*>(collision.entityB);
	ENTROPY_ASSERT(polygonB);

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
	v1 = polyRef->GetOrientationMatrix() * v1 + polyRef->GetPosition();
	v2 = polyRef->GetOrientationMatrix() * v2 + polyRef->GetPosition();

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
		collision.penetration /= static_cast<float>(collision.contacts.size());
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
		vertex = polygonA->GetOrientationMatrix() * vertex + polygonA->GetPosition();
		vertex -= polygonB->GetPosition();
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
	incidentFace[0] = polyInc->GetOrientationMatrix() * polyInc->GetVertex(incidentFaceIdx) + polyInc->GetPosition();
	incidentFaceIdx = (incidentFaceIdx + 1) % polyInc->GetVertexCount();
	incidentFace[1] = polyInc->GetOrientationMatrix() * polyInc->GetVertex(incidentFaceIdx) + polyInc->GetPosition();
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

	ENTROPY_ASSERT(sp != 3);

	return sp;
}

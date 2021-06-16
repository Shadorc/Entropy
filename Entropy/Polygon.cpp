#include "Precompiled.h"

entity::Polygon::Polygon(float x, float y, float width, float height)
	: Entity(x, y)
	, m_Vertices()
	, m_Normals()
	, m_OrientationMatrix(0)
{
	const float halfW = width / 2.0f;
	const float halfH = height / 2.0f;

	m_Vertices.reserve(4);
	m_Vertices.emplace_back(-halfW, -halfH);
	m_Vertices.emplace_back(halfW, -halfH);
	m_Vertices.emplace_back(halfW, halfH);
	m_Vertices.emplace_back(-halfW, halfH);

	m_Normals.reserve(4);
	m_Normals.emplace_back(0.0f, -1.0f);
	m_Normals.emplace_back(1.0f, 0.0f);
	m_Normals.emplace_back(0.0f, 1.0f);
	m_Normals.emplace_back(-1.0f, 0.0f);
}

entity::Polygon::Polygon(float x, float y, std::vector<Vector2>& vertices)
	: Entity(x, y)
	, m_Vertices()
	, m_Normals()
	, m_OrientationMatrix(0)
{
	ENTROPY_ASSERT_WITH_REASON(vertices.size() > 2, "Number of polygon vertices cannot be less than 2");

	// Find the right most point on the hull
	size_t rightMostIdx = 0;
	float highestXCoord = vertices[0].x;
	for (size_t i = 1; i < vertices.size(); ++i)
	{
		const Vector2& vertex = vertices[i];
		if (vertex.x > highestXCoord)
		{
			highestXCoord = vertex.x;
			rightMostIdx = i;
		}
		// If matching x then take farthest negative y
		else if (x == highestXCoord && vertex.y < vertices[rightMostIdx].y)
		{
			rightMostIdx = i;
		}
	}

	size_t hull[24];
	size_t outCount = 0;
	size_t hullIdx = rightMostIdx;
	for (;;)
	{
		hull[outCount] = hullIdx;

		// Search for next index that wraps around the hull by computing cross products to find the
		// most counter-clockwise vertex in the set, given the previos hull index
		size_t nextHullIdx = 0;
		for (size_t i = 1; i < vertices.size(); ++i)
		{
			// Skip if same coordinate as we need three unique
			// points in the set to perform a cross product
			if (nextHullIdx == hullIdx)
			{
				nextHullIdx = i;
				continue;
			}

			// Cross every set of three unique vertices
			// Record each counter clockwise third vertex and add to the output hull
			const Vector2& e1 = vertices[nextHullIdx] - vertices[hull[outCount]];
			const Vector2& e2 = vertices[i] - vertices[hull[outCount]];
			float c = e1.Cross(e2);
			if (c < 0.0f)
			{
				nextHullIdx = i;
			}

			// Cross product is zero then e vectors are on same line
			// therefor want to record vertex farthest along that line
			if (c == 0.0f && e2.LengthSq() > e1.LengthSq())
			{
				nextHullIdx = i;
			}
		}

		++outCount;
		hullIdx = nextHullIdx;

		// Conclude algorithm upon wrap-around
		if (nextHullIdx == rightMostIdx)
		{
			break;
		}
	}

	// Copy vertices into shape's vertices
	for (size_t i = 0; i < outCount; ++i)
	{
		m_Vertices.push_back(vertices[hull[i]]);
	}

	// Compute face normals
	for (size_t i1 = 0; i1 < outCount; ++i1)
	{
		size_t i2 = (i1 + 1) % outCount;
		const Vector2& face = m_Vertices[i2] - m_Vertices[i1];

		ENTROPY_ASSERT_WITH_REASON(!IsZero(face.LengthSq()), "Zero-length edge");

		Vector2 normal = -face.Tangent();
		normal.Normalize();
		m_Normals.push_back(normal);
	}

	ENTROPY_ASSERT(m_Vertices.size() == m_Normals.size());
}

Matrix22 entity::Polygon::GetOrientationMatrix() const
{
	return m_OrientationMatrix;
}

size_t entity::Polygon::GetVertexCount() const
{
	return m_Vertices.size();
}

Vector2 entity::Polygon::GetVertex(size_t index) const
{
	ENTROPY_ASSERT_WITH_REASON(index < m_Vertices.size(), "Vertex index " << index << "out of bound (" << m_Vertices.size() << ")");
	return m_Vertices[index];
}

Vector2 entity::Polygon::GetNormal(size_t index) const
{
	ENTROPY_ASSERT_WITH_REASON(index < m_Normals.size(), "Normal index " << index << "out of bound (" << m_Normals.size() << ")");
	return m_Normals[index];
}

EntityType entity::Polygon::GetType() const
{
	return EntityType::POLYGON;
}

// The extreme point along a direction within a polygon
Vector2 entity::Polygon::GetSupport(const Vector2& dir) const
{
	float bestProjection = -FLT_MAX;
	Vector2 bestVertex;

	for (const Vector2& vertex : m_Vertices)
	{
		const float projection = vertex.Dot(dir);
		if (projection > bestProjection)
		{
			bestVertex = vertex;
			bestProjection = projection;
		}
	}

	return bestVertex;
}

AABB* entity::Polygon::ComputeAABB() const
{
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float maxX = -FLT_MAX;
	float maxY = -FLT_MAX;
	for (const Vector2& vertex : m_Vertices) {
		const Vector2& vector = m_Position + m_OrientationMatrix * vertex;
		if (vector.x < minX)
		{
			minX = vector.x;
		}
		if (vector.y < minY)
		{
			minY = vector.y;
		}
		if (vector.x > maxX)
		{
			maxX = vector.x;
		}
		if (vector.y > maxY)
		{
			maxY = vector.y;
		}
	}
	return ENTROPY_NEW(AABB, minX, minY, maxX, maxY);
}

void entity::Polygon::Rotate(float angle)
{
	m_Orientation += angle;
	m_OrientationMatrix.FromAngle(m_Orientation);
}

void entity::Polygon::Paint() const
{
	glBegin(GL_LINE_LOOP);
	for (const Vector2& vertex : m_Vertices)
	{
		const Vector2& vector = m_Position + m_OrientationMatrix * vertex;
		glVertex2f(vector.x, vector.y);
	}
	glEnd();
}

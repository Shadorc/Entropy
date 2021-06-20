#include "Precompiled.h"

entity::Polygon::Polygon(float x, float y, float width, float height)
	: Entity(x, y)
	, m_Vertices()
	, m_Normals()
	, m_OrientationMatrix(0.0f)
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
	ENTROPY_ASSERT_WITH_REASON(vertices.size() >= 3, "Number of polygon vertices cannot be less than 2");

	m_Vertices.reserve(vertices.size());
	m_Normals.reserve(vertices.size());

	// Find the left most vertex on the hull
	size_t leftMostIdx = 0;
	for (size_t i = 1; i < vertices.size(); ++i)
	{
		const Vector2& vertex = vertices[i];
		if (vertex.x < vertices[leftMostIdx].x)
		{
			leftMostIdx = i;
		}
		// If matching x then take smallest y
		else if (vertex.x == vertices[leftMostIdx].x && vertex.y < vertices[leftMostIdx].y)
		{
			leftMostIdx = i;
		}
	}

	// Convex hull algorithm
	Vector2 pointOnHull = vertices[leftMostIdx];
	Vector2 endpoint;
	do
	{
		if (std::find(m_Vertices.begin(), m_Vertices.end(), pointOnHull) != m_Vertices.end())
		{
			// Prevent infinite loop when 3 or more vertices are colinear
			// This is a degenerate configuration that does not work with this convex hull algorithm
			ENTROPY_ASSERT_WITH_REASON(false, "Some vertices are colinear");
		}

		m_Vertices.push_back(pointOnHull);
		endpoint = vertices[0];
		for (size_t j = 1; j < vertices.size(); ++j)
		{
			Vector2 face1 = pointOnHull - endpoint;
			Vector2 face2 = vertices[j] - endpoint;
			float cross = face1.Cross(face2);
			if (endpoint == pointOnHull || cross > 0.0f)
			{
				endpoint = vertices[j];
			}
		}
		pointOnHull = endpoint;
	} while (endpoint != m_Vertices[0]);

	// Compute face normals
	for (size_t i1 = 0; i1 < m_Vertices.size(); ++i1)
	{
		size_t i2 = (i1 + 1) % m_Vertices.size();
		const Vector2& face = m_Vertices[i2] - m_Vertices[i1];

		ENTROPY_ASSERT_WITH_REASON(!IsZero(face.LengthSq()), "Zero-length edge");

		Vector2 normal = -face.Tangent();
		normal.Normalize();
		m_Normals.push_back(normal);
	}

	ENTROPY_ASSERT(m_Vertices.size() == m_Normals.size());
}

entity::Polygon::~Polygon()
{
	m_Vertices.clear();
	m_Normals.clear();
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
	ENTROPY_ASSERT_WITH_REASON(index < m_Vertices.size(), "Vertex index " << index << " out of bound (" << m_Vertices.size() << ")");
	return m_Vertices[index];
}

Vector2 entity::Polygon::GetNormal(size_t index) const
{
	ENTROPY_ASSERT_WITH_REASON(index < m_Normals.size(), "Normal index " << index << " out of bound (" << m_Normals.size() << ")");
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

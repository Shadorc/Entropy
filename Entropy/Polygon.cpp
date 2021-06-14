#include "Precompiled.h"

entity::Polygon::Polygon(float x, float y, float width, float height)
	: Entity(x, y)
    , m_vertices()
    , m_normals()
    , m_orientationMatrix(0)
{
    const float halfW = width / 2.0f;
    const float halfH = height / 2.0f;

    m_vertices.reserve(4);
    m_vertices.emplace_back(-halfW, -halfH);
    m_vertices.emplace_back(halfW, -halfH);
    m_vertices.emplace_back(halfW, halfH);
    m_vertices.emplace_back(-halfW, halfH);

    m_normals.reserve(4);
    m_normals.emplace_back(0.0f, -1.0f);
    m_normals.emplace_back(1.0f, 0.0f);
    m_normals.emplace_back(0.0f, 1.0f);
    m_normals.emplace_back(-1.0f, 0.0f);
}

Matrix22 entity::Polygon::GetOrientationMatrix() const
{
    return m_orientationMatrix;
}

size_t entity::Polygon::GetVertexCount() const
{
    return m_vertices.size();
}

Vector2 entity::Polygon::GetVertex(size_t index) const
{
    ENTROPY_ASSERT(index < m_vertices.size(), "Vertex index " << index << "out of bound (" << m_vertices.size() << ")");
    return m_vertices[index];
}

Vector2 entity::Polygon::GetNormal(size_t index) const
{
    ENTROPY_ASSERT(index < m_normals.size(), "Normal index " << index << "out of bound (" << m_normals.size() << ")");
    return m_normals[index];
}

AABB entity::Polygon::GetAABB() const
{
    Vector2 start(FLT_MAX, FLT_MAX);
    Vector2 end(-FLT_MAX, -FLT_MAX);
    for (const Vector2& vertex : m_vertices) {
        const Vector2& vector = position + m_orientationMatrix * vertex;
        if (vector.x < start.x)
        {
            start.x = vector.x;
        }
        if (vector.y < start.y)
        {
            start.y = vector.y;
        }
        if (vector.x > end.x)
        {
            end.x = vector.x;
        }
        if (vector.y > end.y)
        {
            end.y = vector.y;
        }
    }
	return AABB(start, end);
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

    for (const Vector2& vertex : m_vertices)
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

void entity::Polygon::Paint() const
{
    glBegin(GL_LINE_LOOP);
    for (const Vector2& vertex : m_vertices)
    {
        const Vector2& vector = position + m_orientationMatrix * vertex;
        glVertex2f(vector.x, vector.y);
    }
    glEnd();
}

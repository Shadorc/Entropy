#include "Precompiled.h"

entity::Polygon::Polygon(float x, float y)
	: Entity(x, y)
{

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
    Vector2 start;
    Vector2 end;
    for (int i = 0; i < m_vertices.size(); ++i) {
        const Vector2& vertex = m_vertices[i];
        if (vertex.x < start.x)
        {
            start.x = vertex.x;
        }
        if (vertex.y < start.y)
        {
            start.y = vertex.y;
        }
        if (vertex.x > end.x)
        {
            end.x = vertex.x;
        }
        if (vertex.y > end.y)
        {
            end.y = vertex.y;
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
        float projection = vertex.Dot(dir);

        if (projection > bestProjection)
        {
            bestVertex = vertex;
            bestProjection = projection;
        }
    }

    return bestVertex;
}

void entity::Polygon::SetBox(float halfW, float halfH)
{
    m_vertices.emplace_back(-halfW, -halfH);
    m_vertices.emplace_back(halfW, -halfH);
    m_vertices.emplace_back(halfW, halfH);
    m_vertices.emplace_back(-halfW, halfH);
    m_normals.emplace_back(0.0f, -1.0f);
    m_normals.emplace_back(1.0f, 0.0f);
    m_normals.emplace_back(0.0f, 1.0f);
    m_normals.emplace_back(-1.0f, 0.0f);
}

void entity::Polygon::Paint() const
{
    glBegin(GL_LINE_LOOP);
    for (const Vector2 vertex : m_vertices)
    {
        const Vector2& vector = position + m_orientationMatrix * vertex;
        glVertex2f(vector.x, vector.y);
    }
    glEnd();
}

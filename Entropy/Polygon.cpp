#include "Precompiled.h"

entity::Polygon::Polygon(float x, float y)
	: Entity(x, y)
{

}

Matrix22 entity::Polygon::GetOrientationMatrix() const
{
    return m_orientationMatrix;
}

unsigned int entity::Polygon::GetVertexCount() const
{
    return m_vertices.size();
}

Vector2 entity::Polygon::GetVertex(unsigned int index) const
{
    ENTROPY_ASSERT(index < m_vertices.size(), "Vertex index " << index << "out of bound (" << m_vertices.size() << ")");
    return m_vertices[index];
}

Vector2 entity::Polygon::GetNormal(unsigned int index) const
{
    ENTROPY_ASSERT(index < m_normals.size(), "Normal index " << index << "out of bound (" << m_normals.size() << ")");
    return m_normals[index];
}

AABB entity::Polygon::GetAABB() const
{
	return AABB(Vector2(), Vector2());
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

    for (unsigned int i = 0; i < m_vertices.size(); ++i)
    {
        const Vector2& vertex = m_vertices[i];
        float projection = vertex.Dot(dir);

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
    for (unsigned int i = 0; i < m_vertices.size(); ++i)
    {
        const Vector2& vector = position + m_orientationMatrix * m_vertices[i];
        glVertex2f(vector.x, vector.y);
    }
    glEnd();
}

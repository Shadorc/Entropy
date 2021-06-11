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
    return m_vertexCount;
}

Vector2 entity::Polygon::GetVertex(unsigned int index) const
{
    return m_vertices[index];
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
    float bestProjection = -FLOAT_INFINITY;
    Vector2 bestVertex;

    for (unsigned int i = 0; i < m_vertexCount; ++i)
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
    for (unsigned int i = 0; i < m_vertexCount; ++i)
    {
        const Vector2& vector = position + m_orientationMatrix * m_vertices[i];
        glVertex2f(vector.x, vector.y);
    }
    glEnd();
}

#include "Precompiled.h"

constexpr float TWICE_PI = 2.0f * 3.14159265f;

entity::Circle::Circle(float x, float y, float radius)
	: Entity(x, y)
	, m_radius(radius)
{

}

float entity::Circle::GetRadius() const
{
    return m_radius;
}

AABB entity::Circle::GetAABB() const
{
    return AABB(
        Vector2(position.x - m_radius, position.y - m_radius),
        Vector2(position.x + m_radius, position.y + m_radius)
    );
}

EntityType entity::Circle::GetType() const
{
    return EntityType::Circle;
}

void entity::Circle::Paint() const
{
    const int segments = 20;
    const Vector2& pos = position.ToNormalizedSpace();
    const float radiusX = m_radius * 2.0f / WIDTH;
    const float radiusY = m_radius * 2.0f / HEIGHT;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++)
    {
        float theta = TWICE_PI * i / segments;
        float x = radiusX * cosf(theta);
        float y = radiusY * sinf(theta);
        glVertex2f(x + pos.x, y + pos.y);
    }
    glEnd();
}

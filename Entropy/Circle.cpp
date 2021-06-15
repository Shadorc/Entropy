#include "Precompiled.h"

entity::Circle::Circle(float x, float y, float radius)
	: Entity(x, y)
	, m_radius(radius)
{

}

float entity::Circle::GetRadius() const
{
    return m_radius;
}

EntityType entity::Circle::GetType() const
{
    return EntityType::CIRCLE;
}

AABB* entity::Circle::ComputeAABB() const
{
    return ENTROPY_NEW(AABB, 
        m_position.x - m_radius, m_position.y - m_radius,
        m_position.x + m_radius, m_position.y + m_radius
    );
}

void entity::Circle::Rotate(float angle)
{
    m_orientation += angle;
}

void entity::Circle::Paint() const
{
    RenderCircle(m_position.x, m_position.y, m_radius);

    // Render line within circle so orientation is visible
    Vector2 radius = Vector2(-sinf(m_orientation), cosf(m_orientation));
    radius *= m_radius;
    radius += m_position;
    RenderLine(m_position.x, m_position.y, radius.x, radius.y);
}

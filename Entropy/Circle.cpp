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
    RenderCircle(position.x, position.y, m_radius);
}

#include "Precompiled.h"

entity::Rectangle::Rectangle(float x, float y, float width, float height)
    : Entity(x, y)
    , m_width(width)
    , m_height(height)
{

}

float entity::Rectangle::GetWidth() const
{
    return m_width;
}

float entity::Rectangle::GetHeight() const
{
    return m_height;
}

AABB entity::Rectangle::GetAABB() const
{
    return AABB(
        Vector2(position.x - m_width / 2.f, position.y - m_height / 2.f),
        Vector2(position.x + m_width / 2.f, position.y + m_height / 2.f)
    );
}

EntityType entity::Rectangle::GetType() const
{
    return EntityType::Rectangle;
}

void entity::Rectangle::Paint() const
{
    RenderRectangle(position.x, position.y, m_width, m_height);
}

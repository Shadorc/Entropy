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

void entity::Rectangle::Paint() const
{
    DrawRectangle(position.x, position.y, m_width, m_height);
}

bool entity::Rectangle::Intersects(const Vector2* point) const
{
    return point->x >= position.x - m_width / 2.0f
        && point->x <= position.x + m_width / 2.0f
        && point->y >= position.y - m_height / 2.0f
        && point->y <= position.y + m_height / 2.0f;
}

bool entity::Rectangle::Intersects(const entity::Rectangle* rectangle) const
{
    return position.x - m_width / 2.0f < rectangle->position.x + rectangle->m_width / 2.0f
        && position.x + m_width / 2.0f > rectangle->position.x - rectangle->m_width / 2.0f
        && position.y - m_height / 2.0f < rectangle->position.y + rectangle->m_height / 2.0f
        && position.y + m_height / 2.0f > rectangle->position.y - rectangle->m_height / 2.0f;
}

bool entity::Rectangle::Intersects(const entity::Circle* circle) const
{
    float closestX = CLAMP(circle->position.x,
        this->position.x - m_width / 2.0f,
        this->position.x + m_width/ 2.0f);
    float closestY = CLAMP(circle->position.y,
        this->position.y - m_height / 2.0f,
        this->position.y + m_height / 2.0f);
    float distanceX = circle->position.x - closestX;
    float distanceY = circle->position.y - closestY;
    return pow(distanceX, 2) + pow(distanceY, 2) < pow(circle->GetRadius(), 2);
}

Vector2 entity::Rectangle::ComputePenetrationVector(const entity::Circle* circle) const
{
    return Vector2();
}

Vector2 entity::Rectangle::ComputePenetrationVector(const entity::Rectangle* rectangle) const
{
    return Vector2();
}


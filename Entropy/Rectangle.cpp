#include "Precompiled.h"

entity::Rectangle::Rectangle(float x, float y, float width, float height):
    Entity(x, y),
    m_width(width),
    m_height(height)
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
        Vector2(m_position->x - m_width / 2.f, m_position->y - m_height / 2.f),
        Vector2(m_position->x + m_width / 2.f, m_position->y + m_height / 2.f)
    );
}

void entity::Rectangle::Paint() const
{
    DrawRectangle(m_position->x, m_position->y, m_width, m_height);
}

bool entity::Rectangle::Intersects(const Vector2* point) const
{
    return point->x >= m_position->x - m_width / 2.0f
        && point->x <= m_position->x + m_width / 2.0f
        && point->y >= m_position->y - m_height / 2.0f
        && point->y <= m_position->y + m_height / 2.0f;
}

bool entity::Rectangle::Intersects(const entity::Rectangle* rectangle) const
{
    return m_position->x - m_width / 2.0f < rectangle->m_position->x + rectangle->m_width / 2.0f
        && m_position->x + m_width / 2.0f > rectangle->m_position->x - rectangle->m_width / 2.0f
        && m_position->y - m_height / 2.0f < rectangle->m_position->y + rectangle->m_height / 2.0f
        && m_position->y + m_height / 2.0f > rectangle->m_position->y - rectangle->m_height / 2.0f;
}

bool entity::Rectangle::Intersects(const entity::Circle* circle) const
{
    float closestX = CLAMP(circle->GetPosition()->x,
        this->m_position->x - m_width / 2.0f,
        this->m_position->x + m_width/ 2.0f);
    float closestY = CLAMP(circle->GetPosition()->y,
        this->m_position->y - m_height / 2.0f,
        this->m_position->y + m_height / 2.0f);
    float distanceX = circle->GetPosition()->x - closestX;
    float distanceY = circle->GetPosition()->y - closestY;
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


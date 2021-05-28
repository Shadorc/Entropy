#include "AABB.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Config.h"
#include "MathUtil.h"
#include <GL/glut.h>
#include "GraphicUtil.h"

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

void entity::Rectangle::Paint() const
{
    DrawRectangle(m_position->m_x, m_position->m_y, m_width, m_height);
}

AABB entity::Rectangle::GetAABB() const
{
    return AABB(
        Vector2(m_position->m_x - m_width / 2.f, m_position->m_y - m_height / 2.f),
        Vector2(m_position->m_x + m_width / 2.f, m_position->m_y + m_height / 2.f)
    );
}

bool entity::Rectangle::Intersects(const Vector2* point) const
{
    return point->m_x >= m_position->m_x - m_width / 2.0f
        && point->m_x <= m_position->m_x + m_width / 2.0f
        && point->m_y >= m_position->m_y - m_height / 2.0f
        && point->m_y <= m_position->m_y + m_height / 2.0f;
}

bool entity::Rectangle::Intersects(const entity::Rectangle* rectangle) const
{
    return m_position->m_x - m_width / 2.0f < rectangle->m_position->m_x + rectangle->m_width / 2.0f
        && m_position->m_x + m_width / 2.0f > rectangle->m_position->m_x - rectangle->m_width / 2.0f
        && m_position->m_y - m_height / 2.0f < rectangle->m_position->m_y + rectangle->m_height / 2.0f
        && m_position->m_y + m_height / 2.0f > rectangle->m_position->m_y - rectangle->m_height / 2.0f;
}

bool entity::Rectangle::Intersects(const entity::Circle* circle) const
{
    float closestX = CLAMP(circle->GetPosition()->m_x,
        this->m_position->m_x - m_width / 2.0f,
        this->m_position->m_x + m_width/ 2.0f);
    float closestY = CLAMP(circle->GetPosition()->m_y,
        this->m_position->m_y - m_height / 2.0f,
        this->m_position->m_y + m_height / 2.0f);
    float distanceX = circle->GetPosition()->m_x - closestX;
    float distanceY = circle->GetPosition()->m_y - closestY;
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


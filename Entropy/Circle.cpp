#include "AABB.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Config.h"
#include <GL/glut.h>

constexpr float TWICE_PI = 2.0f * 3.14159265f;

entity::Circle::Circle(float x, float y, float radius) :
	Entity(x, y),
	m_radius(radius)
{

}

float entity::Circle::GetRadius() const
{
    return m_radius;
}

void entity::Circle::Paint() const
{
    const int segments = 20;
    const Vector2 pos = m_position->ToNormalizedSpace();
    const float radiusX = m_radius * 2.0f / WIDTH;
    const float radiusY = m_radius * 2.0f / HEIGHT;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++)
    {
        float theta = TWICE_PI * i / segments;
        float x = radiusX * cosf(theta);
        float y = radiusY * sinf(theta);
        glVertex2f(x + pos.m_x, y + pos.m_y);
    }
    glEnd();
}

AABB entity::Circle::GetAABB() const
{
    return AABB(
        Vector2(m_position->m_x - m_radius, m_position->m_y - m_radius),
        Vector2(m_position->m_x + m_radius, m_position->m_y + m_radius)
    );
}

bool entity::Circle::Intersects(const Vector2* point) const
{
    return m_position->DistanceSq(*point) <= pow(m_radius, 2);
}

bool entity::Circle::Intersects(const entity::Rectangle* rectangle) const
{
    return rectangle->Intersects(this);
}

bool entity::Circle::Intersects(const entity::Circle* circle) const
{
    float deltaX = m_position->m_x - circle->m_position->m_x;
    float deltaY = m_position->m_y - circle->m_position->m_y;
    float radiusSum = m_radius + circle->m_radius;
    return pow(deltaX, 2) + pow(deltaY, 2) <= pow(radiusSum, 2);
}

Vector2 entity::Circle::ComputePenetrationVector(const entity::Circle* circle) const
{
    float distance = m_position->Distance(*circle->m_position);
    float overlap = distance - (m_radius + circle->m_radius);
    // Multiply the norm by overlap, we don't use Vector2#normalize() because we already have calculated the distance
    return (*circle->m_position - *m_position) / distance * overlap;
}

Vector2 entity::Circle::ComputePenetrationVector(const entity::Rectangle* rectangle) const
{
    return rectangle->ComputePenetrationVector(this) * -1;
}

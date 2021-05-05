#include "Circle.h"
#include "Config.h"
#include <GL/glut.h>

constexpr float TWICE_PI = 2.0f * 3.14159265f;

Circle::Circle(float x, float y, float radius) :
	Entity(x, y),
	m_radius(radius)
{

}

void Circle::Paint() const
{
    const int segments = 20;
    const Vector2 pos = m_position->ToNormalizedSpace();
    const float radiusX = m_radius / WIDTH;
    const float radiusY = m_radius / HEIGHT;

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

AABB Circle::GetAABB() const
{
    return AABB(
        Vector2(m_position->m_x - m_radius, m_position->m_y - m_radius),
        Vector2(m_position->m_x + m_radius, m_position->m_y + m_radius)
    );
}

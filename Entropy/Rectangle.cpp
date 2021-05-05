#include "Rectangle.h"
#include <GL/glut.h>
#include "Config.h"

entity::Rectangle::Rectangle(float x, float y, float width, float height):
    Entity(x, y),
    m_width(width),
    m_height(height)
{
}

void entity::Rectangle::Paint() const
{
    const Vector2 pos = m_position->ToNormalizedSpace();
    float helfWidth = m_width / WIDTH;
    float halfHeight = m_height / HEIGHT;

    glBegin(GL_POLYGON);
    glVertex2f(pos.m_x - helfWidth, pos.m_y - halfHeight);
    glVertex2f(pos.m_x + helfWidth, pos.m_y - halfHeight);
    glVertex2f(pos.m_x + helfWidth, pos.m_y + halfHeight);
    glVertex2f(pos.m_x - helfWidth, pos.m_y + halfHeight);
    glEnd();
}

AABB entity::Rectangle::GetAABB() const
{
    return AABB(
        Vector2(m_position->m_x - m_width / 2.f, m_position->m_y - m_height / 2.f),
        Vector2(m_position->m_x + m_width / 2.f, m_position->m_y + m_height / 2.f)
    );
}

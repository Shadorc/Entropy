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

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(pos.m_x - helfWidth, pos.m_y - halfHeight);
    glVertex2f(pos.m_x + helfWidth, pos.m_y - halfHeight);
    glVertex2f(pos.m_x + helfWidth, pos.m_y + halfHeight);
    glVertex2f(pos.m_x - helfWidth, pos.m_y + halfHeight);
    glEnd();
}

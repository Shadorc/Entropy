#include "Vector2.h"
#include <GL/glut.h>

static void DisplayText(float x, float y, float size, char* str)
{
	float scale = size / 10000.0f;
	glPushMatrix();
		Vector2 pos = Vector2::ToNormalizedSpace(x, y);
		glTranslatef(pos.m_x, pos.m_y, 0);
		glScalef(scale, scale, 1);
		for (char* p = str; *p; p++) {
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
		}
    glPopMatrix();
}

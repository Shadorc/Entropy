#include "Precompiled.h"

void RenderText(float x, float y, const char* str)
{
	glColor3f(0.5f, 0.5f, 0.9f);
	glRasterPos2f(x, y);
	for (int i = 0; i < std::strlen(str); ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *(str + i));
	}
}

void RenderRectangle(float x, float y, float width, float height)
{
	const Vector2& nPosition = Vector2::ToNormalizedSpace(x, y);
	float nWidth = width / WIDTH;
	float nHeight = height / HEIGHT;
	glBegin(GL_LINE_LOOP);
	glVertex2f(nPosition.x - nWidth, nPosition.y - nHeight);
	glVertex2f(nPosition.x + nWidth, nPosition.y - nHeight);
	glVertex2f(nPosition.x + nWidth, nPosition.y + nHeight);
	glVertex2f(nPosition.x - nWidth, nPosition.y + nHeight);
	glEnd();
}

void RenderAABB(const AABB& aabb)
{
	RenderRectangle(aabb.GetX() + aabb.GetWidth() / 2.0f, aabb.GetY() + aabb.GetHeight() / 2.0f, aabb.GetWidth(), aabb.GetHeight());
}

void RenderQuadTree(const QuadTree<Entity>* quadtree)
{
	for (int i = 0; i < INT(Quadrant::SIZE); ++i)
	{
		const auto node = quadtree->GetNode(i);
		if (node != nullptr) {
			RenderAABB(node->GetAABB());
			RenderQuadTree(node);
		}
	}
}

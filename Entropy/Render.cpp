#include "Precompiled.h"

struct Position
{
	float x;
	float y;
};

Position ToNormalizedSpace(float x, float y)
{
	return { 2 * x / WIDTH - 1, 1 - 2 * y / HEIGHT };
}

void RenderLine(float x1, float y1, float x2, float y2)
{
	const Position& startPos = ToNormalizedSpace(x1, y1);
	const Position& endPos = ToNormalizedSpace(x2, y2);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2f(startPos.x, startPos.y);
	glVertex2f(endPos.x, endPos.y);
	glEnd();
}

void RenderText(float x, float y, const char* str)
{
	const Position& pos = ToNormalizedSpace(x, y);
	glColor3f(0.5f, 0.5f, 0.9f);
	glRasterPos2f(pos.x, pos.y);
	for (int i = 0; i < std::strlen(str); ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *(str + i));
	}
}

void RenderRectangle(float x, float y, float width, float height)
{
	const Position& pos = ToNormalizedSpace(x, y);
	float nWidth = width / WIDTH;
	float nHeight = height / HEIGHT;
	glBegin(GL_LINE_LOOP);
	glVertex2f(pos.x - nWidth, pos.y - nHeight);
	glVertex2f(pos.x + nWidth, pos.y - nHeight);
	glVertex2f(pos.x + nWidth, pos.y + nHeight);
	glVertex2f(pos.x - nWidth, pos.y + nHeight);
	glEnd();
}

void RenderCircle(float x, float y, float radius)
{
	const Position& pos = ToNormalizedSpace(x, y);
	const float radiusX = radius * 2.0f / WIDTH;
	const float radiusY = radius * 2.0f / HEIGHT;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < CIRCLE_VERTICES; i++)
	{
		float theta = TWICE_PI * i / CIRCLE_VERTICES;
		float x = radiusX * cosf(theta);
		float y = radiusY * sinf(theta);
		glVertex2f(x + pos.x, y + pos.y);
	}
	glEnd();
}

void RenderAABB(const AABB& aabb)
{
	RenderRectangle(
		aabb.GetX() + aabb.GetWidth() / 2.0f, 
		aabb.GetY() + aabb.GetHeight() / 2.0f, 
		aabb.GetWidth(),
		aabb.GetHeight()
	);
}

void RenderQuadTree(const QuadTree<Entity>* quadtree)
{
	for (int i = 0; i < INT(Quadrant::COUNT); ++i)
	{
		const auto node = quadtree->GetNode(i);
		if (node != nullptr) {
			RenderAABB(node->GetAABB());
			RenderQuadTree(node);
		}
	}
}

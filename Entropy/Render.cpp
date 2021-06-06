#include "Precompiled.h"

constexpr float TWICE_PI = 2.0f * 3.14159265f;
constexpr int MAX_CIRCLE_SEGMENTS = 20;

struct Position
{
	float x;
	float y;
};

Position ToNormalizedSpace(float x, float y)
{
	return { 2 * x / WIDTH - 1, 1 - 2 * y / HEIGHT };
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
	for (int i = 0; i < MAX_CIRCLE_SEGMENTS; i++)
	{
		float theta = TWICE_PI * i / MAX_CIRCLE_SEGMENTS;
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
	for (int i = 0; i < INT(Quadrant::SIZE); ++i)
	{
		const auto node = quadtree->GetNode(i);
		if (node != nullptr) {
			RenderAABB(node->GetAABB());
			RenderQuadTree(node);
		}
	}
}

#include "Precompiled.h"

void RenderLine(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void RenderText(float x, float y, const std::string& str)
{
	glRasterPos2f(x, y);
	for (char character : str)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, character);
	}
}

void RenderRectangle(float x, float y, float width, float height)
{
	const float halfW = width / 2.0f;
	const float halfH = height / 2.0f;
	glBegin(GL_LINE_LOOP);
	glVertex2f(x - halfW, y - halfH);
	glVertex2f(x + halfW, y - halfH);
	glVertex2f(x + halfW, y + halfH);
	glVertex2f(x - halfW, y + halfH);
	glEnd();
}

static constexpr float ARC = TWICE_PI / CIRCLE_VERTICES;
void RenderCircle(float x, float y, float radius)
{
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < CIRCLE_VERTICES; ++i)
	{
		const float theta = i * ARC;
		const float radX = radius * cosf(theta);
		const float radY = radius * sinf(theta);
		glVertex2f(radX + x, radY + y);
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

void RenderQuadTree(const QuadTree<Entity>& quadtree)
{
	for (int i = 0; i < static_cast<int>(Quadrant::COUNT); ++i)
	{
		const auto& node = quadtree.GetNode(i);
		if (node)
		{
			RenderAABB(node->GetAABB());
			RenderQuadTree(*node);
		}
	}
}

#pragma once

#include "Vector2.h"
#include <GL/glut.h>
#include "Config.h"
#include "QuadTree.h"

static void DrawText(float x, float y, float size, char* str)
{
	 // TODO: Do not allocate a vector2
	 Vector2 nPosition = Vector2::ToNormalizedSpace(x, y);
	float scale = size / 10000.0f; // TODO: This is very arbitrary
	glPushMatrix();
	glTranslatef(nPosition.m_x, nPosition.m_y, 0);
	glScalef(scale, scale, 1);
	for (char* p = str; *p; p++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	}
	glPopMatrix();
}

static void DrawRectangle(float x, float y, float width, float height)
{
	// TODO: Do not allocate a vector2
	const Vector2 nPosition = Vector2::ToNormalizedSpace(x, y);
	float nWidth = width / WIDTH;
	float nHeight = height / HEIGHT;
	glBegin(GL_LINE_LOOP);
	glVertex2f(nPosition.m_x - nWidth, nPosition.m_y - nHeight);
	glVertex2f(nPosition.m_x + nWidth, nPosition.m_y - nHeight);
	glVertex2f(nPosition.m_x + nWidth, nPosition.m_y + nHeight);
	glVertex2f(nPosition.m_x - nWidth, nPosition.m_y + nHeight);
	glEnd();
}

static void DrawAABB(const AABB& aabb)
{
	DrawRectangle(aabb.GetX() + aabb.GetWidth() / 2.0f, aabb.GetY() + aabb.GetHeight() / 2.0f, aabb.GetWidth(), aabb.GetHeight());
}

static void DrawQuadTree(const QuadTree<Entity>* quadtree) 
{
	for (int i = 0; i < INT(Quadrant::SIZE); ++i)
	{
		const auto node = quadtree->GetNode(i);
		if (node != nullptr) {
			DrawAABB(*node->GetAABB());
			DrawQuadTree(node);
		}
	}
}

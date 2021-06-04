#pragma once

#include "Vector2.h"
#include <GL/glut.h>
#include "Config.h"
#include "QuadTree.h"
#include <string>

static void DrawText(float x, float y, float size, const std::string& str)
{
	const Vector2& nPosition = Vector2::ToNormalizedSpace(x, y);
	float scale = size / 10000.0f; // TODO: This is very arbitrary
	glPushMatrix();
	glTranslatef(nPosition.x, nPosition.y, 0);
	glScalef(scale, scale, 1);
	for (const char& c : str) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
	}
	glPopMatrix();
}

static void DrawRectangle(float x, float y, float width, float height)
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
			DrawAABB(node->GetAABB());
			DrawQuadTree(node);
		}
	}
}

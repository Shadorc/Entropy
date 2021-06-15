#pragma once

struct AABB;
template<class T> class QuadTree;
class Entity;

struct Position
{
	float x;
	float y;
};

Position ToNormalizedSpace(float x, float y);

void RenderLine(float x1, float y1, float x2, float y2);
void RenderText(float x, float y, const char* str);
void RenderRectangle(float x, float y, float width, float height);
void RenderCircle(float x, float y, float radius);
void RenderAABB(const AABB* aabb);
void RenderQuadTree(const QuadTree<Entity>* quadtree);
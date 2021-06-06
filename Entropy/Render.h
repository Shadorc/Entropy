#pragma once

struct AABB;
template<class T> class QuadTree;
class Entity;

void RenderText(float x, float y, const char* str);
void RenderRectangle(float x, float y, float width, float height);
void RenderCircle(float x, float y, float radius);
void RenderAABB(const AABB& aabb);
void RenderQuadTree(const QuadTree<Entity>* quadtree);
#pragma once

#include "Common.h"
#include "AABB.h"
#include "QuadTree.h"

class CollisionManager
{
private:
	const Sandbox* m_sandbox;
	QuadTree<Entity>* m_quadTree;

	void UpdateQuadTree();
	void CheckCollisions(float delta);

public:
	CollisionManager(const Sandbox* sandbox);

	void Update(float delta);
	void SetRootSize(int width, int height);
};


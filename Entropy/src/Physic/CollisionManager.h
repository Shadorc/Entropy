#pragma once

#include "QuadTree/QuadTree.h"
#include "Physic/Collision.h"
#include "Entity/Entity.h"

class Sandbox;

class CollisionManager
{
public:
	CollisionManager();

	void Update();

	const QuadTree<Entity>& GetRootQuadTree() const;

private:
	QuadTree<Entity> m_QuadTree;

	// Cached vectors
	std::vector<Entity*> m_Entities;
	std::vector<std::tuple<Entity*, Entity*>> m_Pairs;

	void BroadPhase();
	void UpdateQuadTree();
	void SolveCollisions();
	void ApplyImpulses(const Collision& collision);
	void CorrectPosition(const Collision& collision);
};


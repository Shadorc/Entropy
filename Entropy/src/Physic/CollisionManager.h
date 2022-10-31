#pragma once


struct Collision;
class Sandbox;
class Entity;
template<class T> class QuadTree;
template<class T> struct Pair;

class CollisionManager
{
private:
	const Sandbox* m_Sandbox;
	std::unique_ptr<QuadTree<Entity>> m_QuadTree;

	// Cached vectors
	std::vector<Entity*> m_Entities;
	std::vector<Pair<Entity>> m_Pairs;
	std::vector<Pair<Entity>> m_UniquePairs;

	void BroadPhase();
	void UpdateQuadTree();
	void SolveCollisions();
	void ApplyImpulses(const Collision& collision);
	void CorrectPosition(const Collision& collision);

public:
	CollisionManager(const Sandbox* sandbox);

	void Update();

	const QuadTree<Entity>* GetRootQuadTree() const;
	void SetRootSize(int width, int height);
};


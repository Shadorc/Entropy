#pragma once


struct Collision;
class Sandbox;
class Entity;
template<class T> class QuadTree;
template<class T> struct Pair;

class CollisionManager
{
private:
	const Sandbox* m_sandbox;
	QuadTree<Entity>* m_quadTree;

	// Cached vectors
	std::vector<Entity*> m_entities;
	std::vector<Pair<Entity>> m_pairs;
	std::vector<Pair<Entity>> m_uniquePairs;

	void BroadPhase();
	void UpdateQuadTree();
	void CheckCollisions();
	void ResolveCollision(const Collision& manifold);
	void PositionalCorrection(const Collision& manifold);

public:
	CollisionManager(const Sandbox* sandbox);
	~CollisionManager();

	const QuadTree<Entity>* GetRootQuadTree() const;

	void Update();
	void SetRootSize(int width, int height);
};


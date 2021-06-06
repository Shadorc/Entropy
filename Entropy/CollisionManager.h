#pragma once

class Sandbox;
template<class T> class QuadTree;
class Entity;
class CollisionManager
{
private:
	const Sandbox* m_sandbox;
	QuadTree<Entity>* m_quadTree;

	void UpdateQuadTree();
	void CheckCollisions(float deltaTime);

public:
	CollisionManager(const Sandbox* sandbox);
	~CollisionManager();

	const QuadTree<Entity>* GetRootQuadTree() const;

	void Update(float deltaTime);
	void SetRootSize(int width, int height);
};


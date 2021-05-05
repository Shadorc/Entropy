#pragma once

#include "AABB.h"
#include <vector>

enum Quadrant
{
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	TOP_LEFT,
	TOP_RIGHT,
	SIZE,
	INVALID
};

template<typename T>
class QuadTree
{
private:
	const int m_level;
	std::vector<const T*> m_objects;
	const AABB* m_aabb;
	QuadTree* m_nodes[4];

	void Split();
	Quadrant GetQuadrant(const T* object) const;
	std::vector<const T*>& Search(std::vector<const T*>& returnObjects, const T* object) const;
	std::vector<const T*>& SearchChildren(std::vector<const T*>& returnObjects, const T* object) const;

public:
	QuadTree(AABB* aabb);
	QuadTree(int level, AABB* aabb);
	~QuadTree();

	void Clear();
	void Insert(const T* object);
	std::vector<const T*> Search(const T* object) const;
};


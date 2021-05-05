#include "QuadTree.h"

#define MAX_OBJECTS 5
#define MAX_LEVELS 10

template<typename T>
QuadTree<T>::QuadTree(AABB* aabb):
	m_level(0),
	m_aabb(aabb),
	m_nodes() // TODO
{

}

template<typename T>
QuadTree<T>::QuadTree(int level, AABB* aabb) :
	m_level(level),
	m_aabb(aabb),
	m_nodes() // TODO
{

}

template<typename T>
QuadTree<T>::~QuadTree()
{
	Clear();
	delete m_aabb;
}

template<typename T>
void QuadTree<T>::Clear()
{
	m_objects.clear();

	for (int i = 0; i < Quadrant::SIZE; ++i) 
	{
		if (m_nodes[i] != nullptr)
		{
			m_nodes[i]->Clear();
		}
	}
	delete[] m_nodes;
}

template<typename T>
void QuadTree<T>::Split()
{
	int subWidth = m_aabb->GetWidth() / 2.0f;
	int subHeight = m_aabb->GetHeight() / 2.0f;
	const Vector2* topLeft = m_aabb->GetTopLeft();
	const Vector2* bottomRight = m_aabb->GetBottomRight();

	m_nodes[Quadrant::TOP_LEFT] = new QuadTree<T>(m_level + 1, 
		new AABB(*topLeft, *topLeft + Vector2(subWidth, subHeight)));
	m_nodes[Quadrant::TOP_RIGHT] = new QuadTree<T>(m_level + 1,
		new AABB(*topLeft + Vector2(subWidth, 0), *bottomRight - Vector2(0, subHeight)));
	m_nodes[Quadrant::BOTTOM_LEFT] = new QuadTree<T>(m_level + 1,
		new AABB(*topLeft + Vector2(0, subHeight), *bottomRight - Vector2(subWidth, 0)));
	m_nodes[Quadrant::BOTTOM_RIGHT] = new QuadTree<T>(m_level + 1,
		new AABB(*topLeft + Vector2(subWidth, subHeight), *bottomRight));
}

template<typename T>
Quadrant QuadTree<T>::GetQuadrant(const T* object) const
{
	float middleX = m_aabb->GetX() + m_aabb->GetWidth() / 2;
	float middleY = m_aabb->GetY() + m_aabb->GetHeight() / 2;

	AABB aabb = object->GetAABB();
	bool topQuadrant = aabb.GetY() + aabb.GetHeight() < middleY;
	bool bottomQuadrant = aabb.GetY() > middleY;
	if (aabb.GetX() + aabb.GetWidth() < middleX) {
		if (topQuadrant) {
			return Quadrant::TOP_LEFT;
		}
		else if (bottomQuadrant) {
			return Quadrant::BOTTOM_LEFT;
		}
	}
	else if (aabb.GetX() > middleX) {
		if (topQuadrant) {
			return Quadrant::TOP_RIGHT;
		}
		else if (bottomQuadrant) {
			return Quadrant::BOTTOM_RIGHT;
		}
	}
	return Quadrant::INVALID;
}

template<typename T>
std::vector<const T*> QuadTree<T>::Search(const T* object) const
{
	auto vector = std::vector<const T*>();
	Search(vector, object);
	return vector;
}

template<typename T>
std::vector<const T*>& QuadTree<T>::Search(std::vector<const T*>& returnObjects, const T* object) const
{
	Quadrant quadrant = GetQuadrant(object);

	// If the object is between two or more nodes, we check collisions with all children in this node
	if (quadrant == Quadrant::INVALID) {
		return SearchChildren(returnObjects, object);
	}

	if (quadrant != Quadrant::INVALID && m_nodes[0] != nullptr) {
		m_nodes[quadrant]->Search(returnObjects, object);
	}

	returnObjects.insert(returnObjects.end(), m_objects.begin(), m_objects.end());
	return returnObjects;
}

template<typename T>
std::vector<const T*>& QuadTree<T>::SearchChildren(std::vector<const T*>& returnObjects, const T* object) const
{
	if (!m_aabb->IntersectsWith(object->GetAABB()))
	{
		return returnObjects;
	}

	returnObjects.insert(returnObjects.end(), m_objects.begin(), m_objects.end());
	for (const QuadTree* node : m_nodes)
	{
		if (node != nullptr)
		{
			node->SearchChildren(returnObjects, object);
		}
	}

	return returnObjects;
}

template<typename T>
void QuadTree<T>::Insert(const T* object)
{
	if (m_nodes[0] != nullptr) {
		Quadrant quadrant = GetQuadrant(object);
		if (quadrant != Quadrant::INVALID) {
			m_nodes[quadrant]->Insert(object);
			return;
		}
	}

	m_objects.push_back(object);

	if (m_objects.size() > MAX_OBJECTS && m_level < MAX_LEVELS) {
		if (m_nodes[0] == nullptr) {
			Split();
		}

		int i = 0;
		while (i < m_objects.size()) {
			Quadrant quadrant = GetQuadrant(m_objects[i]);
			if (quadrant != Quadrant::INVALID) {
				m_nodes[quadrant]->Insert(m_objects[i]);
				m_objects.erase(m_objects.begin() + i);
			}
			else {
				i++;
			}
		}
	}
}


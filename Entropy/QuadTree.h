#pragma once

#include "Precompiled.h"

constexpr int MAX_OBJECTS = 5;
constexpr int MAX_LEVELS = 10;

enum class Quadrant
{
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	TOP_LEFT,
	TOP_RIGHT,
	COUNT,
	INVALID
};

struct AABB;
template<typename T>
class QuadTree
{
private:
	const int m_level;
	std::vector<T*> m_objects;
	const AABB* m_aabb;
	QuadTree<T>* m_nodes[(int) Quadrant::COUNT];

	void Split()
	{
		float subWidth = m_aabb->GetWidth() / 2.0f;
		float subHeight = m_aabb->GetHeight() / 2.0f;

		m_nodes[(int) Quadrant::TOP_LEFT] = ENTROPY_NEW(QuadTree<T>, m_level + 1,
			ENTROPY_NEW(AABB, m_aabb->minX, m_aabb->minY, m_aabb->minX + subWidth, m_aabb->minY + subHeight));
		m_nodes[(int) Quadrant::TOP_RIGHT] = ENTROPY_NEW(QuadTree<T>, m_level + 1,
			ENTROPY_NEW(AABB, m_aabb->minX + subWidth, m_aabb->minY, m_aabb->maxX, m_aabb->minY + subHeight));
		m_nodes[(int)Quadrant::BOTTOM_LEFT] = ENTROPY_NEW(QuadTree<T>, m_level + 1,
			ENTROPY_NEW(AABB, m_aabb->minX, m_aabb->minY + subHeight, m_aabb->minX + subWidth, m_aabb->maxY));
		m_nodes[(int)Quadrant::BOTTOM_RIGHT] = ENTROPY_NEW(QuadTree<T>, m_level + 1,
			ENTROPY_NEW(AABB, m_aabb->minX + subWidth, m_aabb->minY + subHeight, m_aabb->maxX, m_aabb->maxY));
	}

	Quadrant GetQuadrant(const T* object) const
	{
		float middleX = m_aabb->GetX() + m_aabb->GetWidth() / 2.0f;
		float middleY = m_aabb->GetY() + m_aabb->GetHeight() / 2.0f;

		const AABB* aabb = object->GetAABB();
		bool topQuadrant = aabb->GetY() + aabb->GetHeight() < middleY;
		bool bottomQuadrant = aabb->GetY() > middleY;
		if (aabb->GetX() + aabb->GetWidth() < middleX) 
		{
			if (topQuadrant) 
			{
				return Quadrant::TOP_LEFT;
			}
			else if (bottomQuadrant) 
			{
				return Quadrant::BOTTOM_LEFT;
			}
		}
		else if (aabb->GetX() > middleX)
		{
			if (topQuadrant)
			{
				return Quadrant::TOP_RIGHT;
			}
			else if (bottomQuadrant)
			{
				return Quadrant::BOTTOM_RIGHT;
			}
		}
		return Quadrant::INVALID;
	}

	std::vector<T*> Search(std::vector<T*>& returnObjects, T* object) const
	{
		Quadrant quadrant = GetQuadrant(object);

		// If the object is between two or more nodes, we check collisions with all children in this node
		if (quadrant == Quadrant::INVALID) 
		{
			return SearchChildren(returnObjects, object);
		}

		if (quadrant != Quadrant::INVALID && m_nodes[0] != nullptr)
		{
			m_nodes[(int) quadrant]->Search(returnObjects, object);
		}

		returnObjects.insert(returnObjects.end(), m_objects.begin(), m_objects.end());
		return returnObjects;
	}

	std::vector<T*> SearchChildren(std::vector<T*>& returnObjects, T* object) const
	{
		if (!m_aabb->IntersectsWith(object->GetAABB()))
		{
			return returnObjects;
		}

		returnObjects.insert(returnObjects.end(), m_objects.begin(), m_objects.end());
		for (const QuadTree<T>* node : m_nodes)
		{
			if (node != nullptr)
			{
				node->SearchChildren(returnObjects, object);
			}
		}

		return returnObjects;
	}

public:
	QuadTree(const AABB* aabb) :
		QuadTree(0, aabb)
	{

	}

	QuadTree(int level, const AABB* aabb) :
		m_level(level),
		m_aabb(aabb),
		m_nodes(),
		m_objects()
	{

	}

	~QuadTree()
	{
		ENTROPY_DELETE(m_aabb);
		Clear();
	}

	const QuadTree<T>* GetNode(int i) const
	{
		return m_nodes[i];
	}

	const AABB* GetAABB() const
	{
		return m_aabb;
	}

	void Clear()
	{
		m_objects.clear();

		for (int i = 0; i < (int) Quadrant::COUNT; ++i)
		{
			if (m_nodes[i] != nullptr)
			{
				ENTROPY_DELETE(m_nodes[i]);
			}
		}
	}

	void Insert(T* object)
	{
		if (m_nodes[0] != nullptr)
		{
			Quadrant quadrant = GetQuadrant(object);
			if (quadrant != Quadrant::INVALID)
			{
				m_nodes[(int) quadrant]->Insert(object);
				return;
			}
		}

		m_objects.push_back(object);

		if (m_objects.size() > MAX_OBJECTS && m_level < MAX_LEVELS)
		{
			if (m_nodes[0] == nullptr) 
			{
				Split();
			}

			int i = 0;
			while (i < m_objects.size()) 
			{
				Quadrant quadrant = GetQuadrant(m_objects[i]);
				if (quadrant != Quadrant::INVALID)
				{
					m_nodes[(int) quadrant]->Insert(m_objects[i]);
					m_objects.erase(m_objects.begin() + i);
				}
				else
				{
					i++;
				}
			}
		}
	}

	std::vector<T*> Search(T* object) const
	{
		auto vector = std::vector<T*>();
		Search(vector, object);
		return vector;
	}
};


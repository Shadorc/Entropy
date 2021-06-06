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

/**
 * Source: 
 * https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374
 */
struct AABB;
template<typename T>
class QuadTree
{
private:
	const int m_level;
	std::vector<T*> m_objects;
	const AABB m_aabb;
	QuadTree<T>* m_nodes[INT(Quadrant::COUNT)];

	void Split()
	{
		float subWidth = m_aabb.GetWidth() / 2.0f;
		float subHeight = m_aabb.GetHeight() / 2.0f;
		const Vector2& topLeft = m_aabb.min;
		const Vector2& bottomRight = m_aabb.max;

		m_nodes[INT(Quadrant::TOP_LEFT)] = new QuadTree<T>(m_level + 1,
			AABB(topLeft, topLeft + Vector2(subWidth, subHeight)));
		m_nodes[INT(Quadrant::TOP_RIGHT)] = new QuadTree<T>(m_level + 1,
			AABB(topLeft + Vector2(subWidth, 0), bottomRight - Vector2(0, subHeight)));
		m_nodes[INT(Quadrant::BOTTOM_LEFT)] = new QuadTree<T>(m_level + 1,
			AABB(topLeft + Vector2(0, subHeight), bottomRight - Vector2(subWidth, 0)));
		m_nodes[INT(Quadrant::BOTTOM_RIGHT)] = new QuadTree<T>(m_level + 1,
			AABB(topLeft + Vector2(subWidth, subHeight), bottomRight));
	}

	Quadrant GetQuadrant(const T* object) const
	{
		float middleX = m_aabb.GetX() + m_aabb.GetWidth() / 2.0f;
		float middleY = m_aabb.GetY() + m_aabb.GetHeight() / 2.0f;

		AABB aabb = object->GetAABB();
		bool topQuadrant = aabb.GetY() + aabb.GetHeight() < middleY;
		bool bottomQuadrant = aabb.GetY() > middleY;
		if (aabb.GetX() + aabb.GetWidth() < middleX) 
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
		else if (aabb.GetX() > middleX)
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
			m_nodes[INT(quadrant)]->Search(returnObjects, object);
		}

		returnObjects.insert(returnObjects.end(), m_objects.begin(), m_objects.end());
		return returnObjects;
	}

	std::vector<T*> SearchChildren(std::vector<T*>& returnObjects, T* object) const
	{
		if (!m_aabb.IntersectsWith(object->GetAABB()))
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
	QuadTree(const AABB& aabb) :
		QuadTree(0, aabb)
	{

	}

	QuadTree(int level, const AABB& aabb) :
		m_level(level),
		m_aabb(aabb),
		m_nodes(),
		m_objects()
	{

	}

	~QuadTree()
	{
		Clear();
	}

	const QuadTree<T>* GetNode(int i) const
	{
		return m_nodes[i];
	}

	const AABB GetAABB() const
	{
		return m_aabb;
	}

	void Clear()
	{
		m_objects.clear();

		for (int i = 0; i < INT(Quadrant::COUNT); ++i)
		{
			if (m_nodes[i] != nullptr)
			{
				m_nodes[i]->Clear();
				delete m_nodes[i];
				m_nodes[i] = nullptr;
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
				m_nodes[INT(quadrant)]->Insert(object);
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
					m_nodes[INT(quadrant)]->Insert(m_objects[i]);
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


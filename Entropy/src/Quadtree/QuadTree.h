#pragma once

#include "Quadtree/AABB.h"

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

template<typename T>
class QuadTree
{
private:
	const int m_Level;
	std::vector<T*> m_Objects;
	const AABB m_Aabb;
	std::unique_ptr<QuadTree<T>> m_Nodes[static_cast<int>(Quadrant::COUNT)];

	void Split()
	{
		float subWidth = m_Aabb.GetWidth() / 2.0f;
		float subHeight = m_Aabb.GetHeight() / 2.0f;

		m_Nodes[static_cast<int>(Quadrant::TOP_LEFT)] = std::make_unique<QuadTree<T>>(m_Level + 1,
			AABB(m_Aabb.minX, m_Aabb.minY, m_Aabb.minX + subWidth, m_Aabb.minY + subHeight));
		m_Nodes[static_cast<int>(Quadrant::TOP_RIGHT)] = std::make_unique<QuadTree<T>>(m_Level + 1,
			AABB(m_Aabb.minX + subWidth, m_Aabb.minY, m_Aabb.maxX, m_Aabb.minY + subHeight));
		m_Nodes[static_cast<int>(Quadrant::BOTTOM_LEFT)] = std::make_unique<QuadTree<T>>(m_Level + 1,
			AABB(m_Aabb.minX, m_Aabb.minY + subHeight, m_Aabb.minX + subWidth, m_Aabb.maxY));
		m_Nodes[static_cast<int>(Quadrant::BOTTOM_RIGHT)] = std::make_unique<QuadTree<T>>(m_Level + 1,
			AABB(m_Aabb.minX + subWidth, m_Aabb.minY + subHeight, m_Aabb.maxX, m_Aabb.maxY));
	}

	Quadrant GetQuadrant(const T* object) const
	{
		float middleX = m_Aabb.GetX() + m_Aabb.GetWidth() / 2.0f;
		float middleY = m_Aabb.GetY() + m_Aabb.GetHeight() / 2.0f;

		const AABB& aabb = object->GetAABB();
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

		if (quadrant != Quadrant::INVALID && m_Nodes[0])
		{
			m_Nodes[static_cast<int>(quadrant)]->Search(returnObjects, object);
		}

		returnObjects.insert(returnObjects.end(), m_Objects.begin(), m_Objects.end());
		return returnObjects;
	}

	std::vector<T*> SearchChildren(std::vector<T*>& returnObjects, T* object) const
	{
		if (!m_Aabb.IntersectsWith(object->GetAABB()))
		{
			return returnObjects;
		}

		returnObjects.insert(returnObjects.end(), m_Objects.begin(), m_Objects.end());
		for (const auto& node : m_Nodes)
		{
			if (node)
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
		m_Level(level),
		m_Aabb(aabb),
		m_Nodes(),
		m_Objects()
	{

	}

	~QuadTree()
	{
		Clear();
	}

	const QuadTree<T>* GetNode(int i) const
	{
		return m_Nodes[i].get();
	}

	const AABB& GetAABB() const
	{
		return m_Aabb;
	}

	void Clear()
	{
		m_Objects.clear();

		for (int i = 0; i < static_cast<int>(Quadrant::COUNT); ++i)
		{
			m_Nodes[i].reset();
		}
	}

	void Insert(T* object)
	{
		if (m_Nodes[0])
		{
			Quadrant quadrant = GetQuadrant(object);
			if (quadrant != Quadrant::INVALID)
			{
				m_Nodes[static_cast<int>(quadrant)]->Insert(object);
				return;
			}
		}

		m_Objects.push_back(object);

		if (m_Objects.size() > MAX_OBJECTS && m_Level < MAX_LEVELS)
		{
			if (!m_Nodes[0])
			{
				Split();
			}

			int i = 0;
			while (i < m_Objects.size())
			{
				Quadrant quadrant = GetQuadrant(m_Objects[i]);
				if (quadrant != Quadrant::INVALID)
				{
					m_Nodes[static_cast<int>(quadrant)]->Insert(m_Objects[i]);
					m_Objects.erase(m_Objects.begin() + i);
				}
				else
				{
					++i;
				}
			}
		}
	}

	std::vector<T*> Search(T* object) const
	{
		std::vector<T*> vector;
		vector.reserve(MAX_OBJECTS);
		Search(vector, object);
		return vector;
	}
};


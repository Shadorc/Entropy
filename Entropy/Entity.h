#pragma once

#include "AABBObject.h"
#include "Vector2.h"
#include <vector>

class Component;
class RigidBodyComponent;
namespace entity 
{
	class Circle;
	class Rectangle;
}

enum class EntityType
{
	CIRCLE,
	RECTANGLE,
	COUNT
};

class Entity: public AABBObject
{
private:
	mutable RigidBodyComponent* m_rigidBodyComponentCache;

protected:
	const unsigned int m_id;
	std::vector<Component*> m_components;

public:
	Entity(float x, float y);
	~Entity();

	Vector2 position;
	Vector2 velocity;

	unsigned int GetId() const;
	virtual AABB GetAABB() const = 0;
	virtual EntityType GetType() const = 0;

	template<typename T>
	T* GetComponent() const
	{
		for (Component* itr : m_components)
		{
			T* component = dynamic_cast<T*>(itr);
			if (component != nullptr)
			{
				return component;
			}
		}
		return nullptr;
	}
	RigidBodyComponent* GetRigidBodyComponent() const;

	void AddComponent(Component* component);
	virtual void Update(float deltaTime);
	virtual void Paint() const = 0;

	bool operator==(Entity& other) const;
	bool operator!=(Entity& other) const;
};


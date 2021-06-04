#pragma once

#include "AABBObject.h"
#include "Vector2.h"
#include <vector>

// TODO: Precompiled header
class Component;
class RigidBodyComponent;
namespace entity 
{
	class Circle;
	class Rectangle;
}

enum class EntityType
{
	Circle,
	Rectangle,
	Count
};

class Entity: public AABBObject
{
private:
	mutable RigidBodyComponent* m_rigidBodyComponentCache;

protected:
	const unsigned int m_id;
	// TODO: Don't use pointers
	Vector2* m_position;
	Vector2* m_velocity;
	std::vector<Component*> m_components;

public:
	Entity(float x, float y);
	~Entity();

	unsigned int GetId() const;
	Vector2* GetPosition() const;
	Vector2* GetVelocity() const;
	virtual AABB GetAABB() const = 0;

	template<typename T>
	T* GetComponent() const
	{
		for (Component* itr : m_components)
		{
			T* component = reinterpret_cast<T*>(itr);
			if (component != nullptr)
			{
				return component;
			}
		}
		return nullptr;
	}
	RigidBodyComponent* GetRigidBodyComponent() const;

	void AddComponent(Component* component);
	virtual void Update(float delta);
	virtual void Paint() const = 0;

	bool Intersects(const Entity* other) const;
	virtual bool Intersects(const Vector2* point) const = 0;
	virtual bool Intersects(const entity::Rectangle* rectangle) const = 0;
	virtual bool Intersects(const entity::Circle* circle) const = 0;

	Vector2 ComputePenetrationVector(const Entity* other) const;
	virtual Vector2 ComputePenetrationVector(const entity::Rectangle* rectangle) const = 0;
	virtual Vector2 ComputePenetrationVector(const entity::Circle* circle) const = 0;

	bool operator==(Entity& other) const;
};


#pragma once

#include "AABBObject.h"
#include "Vector2.h"
#include <vector>


enum class EntityType
{
	CIRCLE,
	POLYGON,
	COUNT
};

class Component;
class RigidbodyComponent;
class Entity: public AABBObject
{
private:
	mutable RigidbodyComponent* m_rigidbodyComponentCache;

protected:
	const unsigned int m_id;
	std::vector<Component*> m_components;

public:
	Entity(float x, float y);
	~Entity();

	Vector2 position;
	Vector2 velocity;
	float orientation;
	float angularVelocity;
	float torque;

	unsigned int GetId() const;
	// TODO: Use GetAABB(AABB& aabb) to avoid re-allocating aabb
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
	RigidbodyComponent* GetRigidbodyComponent() const;

	void AddComponent(Component* component);
	void ApplyImpulse(const Vector2& impulse, const Vector2& contactVector);
	virtual void Update(float deltaTime);
	virtual void Paint() const = 0;

	bool operator==(Entity& other) const;
};


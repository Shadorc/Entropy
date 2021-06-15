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
	mutable RigidbodyComponent* m_RigidbodyComponentCache;
	mutable AABB* m_AabbCache;

protected:
	const unsigned int m_Id;
	std::vector<Component*> m_Components;
	Vector2 m_Position;
	float m_Orientation;

public:
	Entity(float x, float y);
	~Entity();

	Vector2 velocity;
	Vector2 force;
	float angularVelocity;
	float torque;

	unsigned int GetId() const;
	const Vector2 GetPosition() const;
	const AABB* GetAABB() const;
	virtual EntityType GetType() const = 0;

	template<typename T>
	T* GetComponent() const
	{
		for (Component* itr : m_Components)
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
	virtual AABB* ComputeAABB() const = 0;

	void ApplyImpulse(const Vector2& impulse, const Vector2& contactVector);
	void Translate(const Vector2& vector);
	virtual void Rotate(float angle) = 0;

	virtual void Update(float deltaTime);
	virtual void Paint() const = 0;

	bool operator==(Entity& other) const;
};


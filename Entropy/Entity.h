#pragma once

#include <vector>

#include "AABBObject.h"
#include "Vector2.h"

enum class EntityType
{
	CIRCLE,
	POLYGON,
	COUNT
};

class Component;
class RigidbodyComponent;
class Entity : public AABBObject
{
private:
	mutable RigidbodyComponent* m_RigidbodyComponentCache;
	mutable AABB* m_AabbCache;

protected:
	const uint m_Id;
	std::vector<Component*> m_Components;
	Vector2 m_Position;
	float m_Orientation;

public:
	Entity(float x, float y);
	virtual ~Entity();

	Vector2 velocity;
	float angularVelocity;

	uint GetId() const;
	const Vector2 GetPosition() const;
	const float GetOrientation() const;
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

	void Translate(const Vector2& vector);
	virtual void Rotate(float angle) = 0;

	virtual void Update(float deltaTime);
	virtual void Paint() const = 0;

	bool operator==(Entity& other) const;
};


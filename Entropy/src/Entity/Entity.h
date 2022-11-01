#pragma once

#include <vector>
#include "Quadtree/AABBObject.h"
#include "Math/Vector2.h"
#include "Component/RigidBodyComponent.h"

enum class EntityType
{
	CIRCLE,
	POLYGON,
	COUNT
};

class Entity : public AABBObject
{
public:
	Entity(float x, float y);

	Vector2 velocity;
	float angularVelocity;

	uint GetId() const;
	const Vector2 GetPosition() const;
	const float GetOrientation() const;
	const AABB& GetAABB() const override;
	virtual EntityType GetType() const = 0;

	template<typename T>
	T* GetComponent() const
	{
		for (const auto& itr : m_Components)
		{
			T* component = dynamic_cast<T*>(itr.get());
			if (component)
			{
				return component;
			}
		}
		return nullptr;
	}
	RigidbodyComponent* GetRigidbodyComponent() const;

	void AddComponent(std::unique_ptr<Component> component);
	virtual std::unique_ptr<AABB> ComputeAABB() const = 0;

	void Translate(const Vector2& vector);
	virtual void Rotate(float angle) = 0;
	virtual void Update(float deltaTime);
	virtual void Paint() const = 0;

	bool operator==(Entity& other) const;

protected:
	const uint m_Id;
	std::vector<std::unique_ptr<Component>> m_Components;
	Vector2 m_Position;
	float m_Orientation;

private:
	mutable std::unique_ptr<RigidbodyComponent> m_RigidbodyComponentCache;
	mutable std::unique_ptr<AABB> m_AabbCache;
};

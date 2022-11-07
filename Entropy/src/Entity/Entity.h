#pragma once

#include <unordered_map>
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
	const Vector2& GetPosition() const;
	const float GetOrientation() const;
	const AABB& GetAABB() const override;
	virtual EntityType GetType() const = 0;

	template<typename T, typename... Args>
	void AddComponent(Args&&... args)
	{
		ENTROPY_ASSERT_WITH_REASON(!HasComponent<T>(), "Component already exists");
		m_Components[typeid(T).name()] = std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	bool HasComponent() const
	{
		return m_Components.find(typeid(T).name()) != m_Components.cend();
	}

	template<typename T>
	T& GetComponent() const
	{
		ENTROPY_ASSERT_WITH_REASON(HasComponent<T>(), "Component not found");
		const char* key = typeid(T).name();
		const auto& ptr = m_Components.at(key).get();
		return dynamic_cast<T&>(*ptr);
	}
	
	virtual std::unique_ptr<AABB> ComputeAABB() const = 0;

	void Translate(const Vector2& vector);
	virtual void Rotate(float angle) = 0;
	virtual void Update(float deltaTime);
	virtual void Paint() const = 0;

	bool operator==(Entity& other) const;

protected:
	const uint m_Id;
	std::unordered_map<const char*, std::unique_ptr<Component>> m_Components;
	Vector2 m_Position;
	float m_Orientation;

private:
	mutable std::unique_ptr<AABB> m_Aabb;
};

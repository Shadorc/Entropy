#pragma once

#include "Vector2.h"
#include "AABBObject.h"
#include <vector>

class Component;
class RigidBodyComponent;
class GravityComponent;
class Entity: public AABBObject
{
protected:
	const unsigned int m_id;
	Vector2* m_position;
	Vector2* m_velocity;
	std::vector<Component*> m_components;

public:
	Entity(float x, float y);
	~Entity();

	Vector2* GetPosition() const;
	Vector2* GetVelocity() const;

	RigidBodyComponent* GetRigidBodyComponent() const;
	GravityComponent* GetGravityComponent() const;

	void AddComponent(Component* component);
	virtual void Update(float delta);
	virtual void Paint() const = 0;

	bool operator==(Entity& other) const;
};


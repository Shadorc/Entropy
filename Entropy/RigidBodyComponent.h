#pragma once

#include "Entity.h"
#include "Component.h"

class RigidBodyComponent: public Component
{
private:
	float m_mass;
	std::vector<Vector2*> m_forces;
	Vector2* m_acceleration;

public:
	RigidBodyComponent(const Entity* entity, float mass);
	~RigidBodyComponent();

	void AddForce(Vector2* force);
	void Update(float delta) override;

	float GetMass() const;
};


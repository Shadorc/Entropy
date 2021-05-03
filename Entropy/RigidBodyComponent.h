#pragma once

#include "Component.h"
#include "Vector2.h"
#include <vector>

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


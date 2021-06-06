#pragma once

#include <vector>

#include "Component.h"
#include "Vector2.h"
#include "MassData.h"
#include "Material.h"
#include "FrictionData.h"

class Entity;
class RigidBodyComponent: public Component
{
private:
	MassData m_massData;
	Material m_material;
	FrictionData m_frictionData;
	std::vector<Vector2> m_forces;

	void ComputeMass();

public:
	RigidBodyComponent(Entity* entity, Material material);

	void AddForce(Vector2 force);
	void Update(float deltaTime) override;

	MassData GetMassData() const;
	Material GetMaterial() const;
	FrictionData GetFrictionData() const;
};


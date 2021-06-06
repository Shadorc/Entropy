#pragma once

#include <vector>

#include "Component.h"
#include "Vector2.h"
#include "MassData.h"
#include "Material.h"
#include "FrictionData.h"

enum class RigidbodyType
{
	STATIC, 
	DYNAMIC,
	COUNT
};

class Entity;
class RigidBodyComponent: public Component
{
private:
	RigidbodyType m_type;
	MassData m_massData;
	Material m_material;
	FrictionData m_frictionData;
	std::vector<Vector2> m_forces;

	void ComputeMass();

public:
	RigidBodyComponent(Entity* entity);
	RigidBodyComponent(Entity* entity, float density);
	RigidBodyComponent(Entity* entity, RigidbodyType type);
	~RigidBodyComponent();

	void AddForce(Vector2 force);
	void Update(float deltaTime) override;

	RigidbodyType GetType() const;
	MassData GetMassData() const;
	Material GetMaterial() const;
	FrictionData GetFrictionData() const;

	bool IsStatic() const;
};


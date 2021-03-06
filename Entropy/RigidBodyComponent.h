#pragma once

#include "Component.h"
#include "Vector2.h"
#include "MassData.h"
#include "MaterialData.h"
#include "FrictionData.h"

class Entity;
class RigidbodyComponent : public Component
{
private:
	MassData m_MassData;
	MaterialData m_MaterialData;
	FrictionData m_FrictionData;
	Vector2 m_Force;
	float m_Torque;

	void ComputeMass();

public:
	RigidbodyComponent(Entity* entity);
	RigidbodyComponent(Entity* entity, MaterialData materialData, FrictionData frictionData);

	void ApplyImpulse(const Vector2& impulse, const Vector2& contact);
	void AddForce(const Vector2& force);
	void AddTorque(float torque);
	void Update(float deltaTime) override;

	MassData GetMassData() const;
	MaterialData GetMaterialData() const;
	FrictionData GetFrictionData() const;
};


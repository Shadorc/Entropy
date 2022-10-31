#pragma once

#include "Component/Component.h"
#include "Math/Vector2.h"
#include "Physic/Data/MassData.h"
#include "Physic/Data/MaterialData.h"
#include "Physic/Data/FrictionData.h"

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

	void ApplyImpulse(const Vector2& impulse, const Vector2& contact) const;
	void AddForce(const Vector2& force);
	void AddTorque(float torque);
	void Update(float deltaTime) override;

	MassData GetMassData() const;
	MaterialData GetMaterialData() const;
	FrictionData GetFrictionData() const;
};


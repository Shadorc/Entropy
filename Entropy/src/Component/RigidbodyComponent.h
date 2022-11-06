#pragma once

#include "Component/Component.h"
#include "Math/Vector2.h"
#include "Physic/Data/MassData.h"
#include "Physic/Data/MaterialData.h"
#include "Physic/Data/FrictionData.h"

class RigidbodyComponent : public Component
{
public:
	RigidbodyComponent(const std::weak_ptr<Entity>& entity);
	RigidbodyComponent(const std::weak_ptr<Entity>& entity, MaterialData materialData, FrictionData frictionData);

	void ApplyImpulse(const Vector2& impulse, const Vector2& contact) const;
	void AddForce(const Vector2& force);
	void AddTorque(float torque);
	void Update(float deltaTime) override;

	const MassData& GetMassData() const;
	const MaterialData& GetMaterialData() const;
	const FrictionData& GetFrictionData() const;

private:
	const MaterialData m_MaterialData;
	const FrictionData m_FrictionData;
	const MassData m_MassData;
	Vector2 m_Force;
	float m_Torque;

	MassData ComputeMass();
};


#pragma once

#include <vector>

#include "Component.h"
#include "Vector2.h"
#include "MassData.h"
#include "MaterialData.h"
#include "FrictionData.h"

class Entity;
class RigidbodyComponent: public Component
{
private:
	MassData m_MassData;
	MaterialData m_MaterialData;
	FrictionData m_FrictionData;

	void ComputeMass();

public:
	RigidbodyComponent(Entity* entity, MaterialData material);

	void AddForce(const Vector2& force);
	void Update(float deltaTime) override;

	MassData GetMassData() const;
	MaterialData GetMaterialData() const;
	FrictionData GetFrictionData() const;
};


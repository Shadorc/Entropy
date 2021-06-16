#pragma once

#include "Component.h"

class Entity;
class GravityComponent : public Component
{
public:
	static const Vector2 GRAVITY;

	GravityComponent(Entity* entity);

	void Update(float deltaTime) override;
};


#pragma once

#include "Component.h"

class Entity;
class GravityComponent: public Component
{
public:
	GravityComponent(Entity* entity);

	void Update(float deltaTime) override;
};


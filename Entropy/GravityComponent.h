#pragma once

#include "Entity.h"
#include "Component.h"

class GravityComponent: public Component
{
public:
	GravityComponent(const Entity* entity);

	void Update(float delta) override;
};


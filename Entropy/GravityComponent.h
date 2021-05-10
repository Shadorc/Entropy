#pragma once

#include "Component.h"

class Entity;
class GravityComponent: public Component
{
public:
	GravityComponent(const Entity* entity);

	void Update(float delta) override;
};


#pragma once

#include "Component/Component.h"

class GravityComponent : public Component
{
public:
	static const Vector2 GRAVITY;

	GravityComponent(const std::weak_ptr<Entity>& entity);

	void Update(float deltaTime) override;
};


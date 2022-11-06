#pragma once

#include "Entity/Entity.h"

class Component
{
public:
	Component(const std::weak_ptr<Entity>& entity);

	virtual void Update(float deltaTime) = 0;

protected:
	const std::weak_ptr<Entity> m_Entity;
};


#pragma once

#include "Entity/Entity.h"

class Component
{
public:
	Component(Entity* entity);

	const Entity* GetEntity() const;

	virtual void Update(float deltaTime) = 0;

protected:
	Entity* m_Entity;
};


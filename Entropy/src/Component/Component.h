#pragma once

#include "Entity/Entity.h"

class Component
{
protected:
	Entity* m_Entity;

public:
	Component(Entity* entity);

	const Entity* GetEntity() const;

	virtual void Update(float deltaTime) = 0;
};


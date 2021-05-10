#pragma once

class Entity;
class Component
{
protected:
	const Entity* m_entity;

public:
	Component(const Entity* entity);

	const Entity* GetEntity() const;

	virtual void Update(float delta) = 0;
};


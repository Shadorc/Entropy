#include "Precompiled.h"

Component::Component(Entity* entity)
	: m_entity(entity)
{
}

const Entity* Component::GetEntity() const
{
	return m_entity;
}

#include "Precompiled.h"

Component::Component(const Entity* entity):
	m_entity(entity)
{
}

const Entity* Component::GetEntity() const
{
	return m_entity;
}

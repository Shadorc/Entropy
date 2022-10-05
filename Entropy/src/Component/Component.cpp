#include "Precompiled.h"

Component::Component(Entity* entity)
	: m_Entity(entity)
{
}

const Entity* Component::GetEntity() const
{
	return m_Entity;
}

#include "Precompiled.h"

Component::Component(const std::weak_ptr<Entity>& entity)
	: m_Entity(entity)
{

}

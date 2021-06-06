#include "Precompiled.h"

static int id = 0;

Entity::Entity(float x, float y) 
	: m_id(id++)
	, position(x, y)
	, velocity()
	, m_rigidbodyComponentCache(nullptr)
{

}

Entity::~Entity()
{
	delete m_rigidbodyComponentCache;
	for (Component* component : m_components)
	{
		delete component;
	}
	m_components.clear();
}

// Cache Rigidbody component
RigidbodyComponent* Entity::GetRigidbodyComponent() const
{
	if (m_rigidbodyComponentCache == nullptr)
	{
		m_rigidbodyComponentCache = GetComponent<RigidbodyComponent>();
	}
	return m_rigidbodyComponentCache;
}

void Entity::AddComponent(Component* component)
{
	m_components.push_back(component);
}

unsigned int Entity::GetId() const
{
	return m_id;
}

void Entity::Update(float deltaTime)
{
	for (Component* component : m_components)
	{
		component->Update(deltaTime);
	}
}

bool Entity::operator==(Entity& other) const
{
	return m_id == other.m_id;
}

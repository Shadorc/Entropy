#include "Precompiled.h"

static int id = 0;

Entity::Entity(float x, float y) 
	: m_id(id++)
	, position(x, y)
	, velocity()
	, m_rigidBodyComponentCache(nullptr)
{

}

Entity::~Entity()
{
	delete m_rigidBodyComponentCache;
	for (Component* component : m_components)
	{
		delete component;
	}
	m_components.clear();
}

// Cache RigidBody component
RigidBodyComponent* Entity::GetRigidBodyComponent() const
{
	if (m_rigidBodyComponentCache == nullptr)
	{
		m_rigidBodyComponentCache = GetComponent<RigidBodyComponent>();
	}
	return m_rigidBodyComponentCache;
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

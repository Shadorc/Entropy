#include "Precompiled.h"

static uint s_Id = 0;

Entity::Entity(float x, float y)
	: m_Id(s_Id++)
	, m_Position(x, y)
	, velocity()
	, m_Orientation(0.0f)
	, angularVelocity(0.0f)
	, m_RigidbodyComponentCache(nullptr)
	, m_AabbCache(nullptr)
{

}

Entity::~Entity()
{
	ENTROPY_DELETE(m_AabbCache);
	for (Component* component : m_Components)
	{
		ENTROPY_DELETE(component);
	}
	m_Components.clear();
}

// Cache Rigidbody component
RigidbodyComponent* Entity::GetRigidbodyComponent() const
{
	if (m_RigidbodyComponentCache == nullptr)
	{
		m_RigidbodyComponentCache = GetComponent<RigidbodyComponent>();
	}
	return m_RigidbodyComponentCache;
}

void Entity::AddComponent(Component* component)
{
	m_Components.push_back(component);
}

void Entity::Translate(const Vector2& vector)
{
	ENTROPY_DELETE(m_AabbCache);
	m_Position += vector;
}

uint Entity::GetId() const
{
	return m_Id;
}

const Vector2 Entity::GetPosition() const
{
	return m_Position;
}

const float Entity::GetOrientation() const
{
	return m_Orientation;
}

const AABB* Entity::GetAABB() const
{
	if (m_AabbCache == nullptr)
	{
		m_AabbCache = ComputeAABB();
	}
	return m_AabbCache;
}

void Entity::Update(float deltaTime)
{
	for (Component* component : m_Components)
	{
		component->Update(deltaTime);
	}
}

bool Entity::operator==(Entity& other) const
{
	return m_Id == other.m_Id;
}

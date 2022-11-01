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

// Cache Rigidbody component
RigidbodyComponent* Entity::GetRigidbodyComponent() const
{
	if (!m_RigidbodyComponentCache)
	{
		m_RigidbodyComponentCache.reset(GetComponent<RigidbodyComponent>());
	}
	return m_RigidbodyComponentCache.get();
}

void Entity::AddComponent(std::unique_ptr<Component> component)
{
	m_Components.emplace_back(std::move(component));
}

void Entity::Translate(const Vector2& vector)
{
	m_AabbCache.reset();
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

const AABB& Entity::GetAABB() const
{
	if (!m_AabbCache)
	{
		m_AabbCache = std::move(ComputeAABB());
	}
	return *m_AabbCache.get();
}

void Entity::Update(float deltaTime)
{
	for (const auto& component : m_Components)
	{
		component->Update(deltaTime);
	}
}

bool Entity::operator==(Entity& other) const
{
	return m_Id == other.m_Id;
}

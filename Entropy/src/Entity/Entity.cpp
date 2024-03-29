#include "Precompiled.h"

static uint s_Id = 0;

Entity::Entity(float x, float y)
	: m_Id(s_Id++)
	, m_Position(x, y)
	, velocity()
	, m_Orientation(0.0f)
	, angularVelocity(0.0f)
	, m_Aabb(nullptr)
{

}

void Entity::Translate(const Vector2& vector)
{
	m_Aabb.reset();
	m_Position += vector;
}

uint Entity::GetId() const
{
	return m_Id;
}

const Vector2& Entity::GetPosition() const
{
	return m_Position;
}

const float Entity::GetOrientation() const
{
	return m_Orientation;
}

const AABB& Entity::GetAABB() const
{
	if (!m_Aabb)
	{
		m_Aabb = std::move(ComputeAABB());
	}
	return *m_Aabb.get();
}

void Entity::Update(float deltaTime)
{
	for (const auto& pair : m_Components)
	{
		pair.second->Update(deltaTime);
	}
}

bool Entity::operator==(Entity& other) const
{
	return m_Id == other.m_Id;
}

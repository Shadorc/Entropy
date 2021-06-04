#include "Precompiled.h"

RigidBodyComponent::RigidBodyComponent(Entity* entity)
	: RigidBodyComponent(entity, RigidbodyType::STATIC)
{

}

RigidBodyComponent::RigidBodyComponent(Entity* entity, RigidbodyType type)
	: Component(entity)
	, m_type(type)
	, m_mass(1)
	, m_invMass(1)
	, m_acceleration()
{

}

RigidBodyComponent::~RigidBodyComponent()
{
	m_forces.clear();
}

void RigidBodyComponent::Update(float deltaTime)
{
	Vector2 forcesSum;

	auto it = m_forces.begin();
	while (it != m_forces.end())
	{
		Force& force = *it;
		forcesSum += force.vector;
		if (force.isInstant)
		{
			it = m_forces.erase(it);
		}
		else
		{
			++it;
		}
	}
	
	m_acceleration = forcesSum / m_mass;
	m_entity->velocity += m_acceleration * deltaTime;

	float speedLengthSq = m_entity->velocity.LengthSq();
	if (speedLengthSq < Vector2::EPSILON_SQ)
	{
		m_entity->velocity.Reset();
	}
	else
	{
		m_entity->position += m_entity->velocity * deltaTime;
	}
}

void RigidBodyComponent::AddForce(Vector2& force)
{
	m_forces.emplace_back(force, false);
}

void RigidBodyComponent::AddInstantForce(Vector2& force)
{
	m_forces.emplace_back(force, true);
}

RigidbodyType RigidBodyComponent::GetType() const
{
	return m_type;
}

float RigidBodyComponent::GetMass() const
{
	return m_mass;
}

float RigidBodyComponent::GetInvMass() const
{
	return m_invMass;
}

float RigidBodyComponent::GetRestitution() const
{
	return 0.2f;
}

bool RigidBodyComponent::IsStatic() const
{
	return m_type == RigidbodyType::STATIC;
}


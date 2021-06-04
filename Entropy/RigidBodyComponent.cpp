#include "Entity.h"
#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(const Entity* entity, const Type type, const float mass) :
	Component(entity),
	m_type(type),
	m_mass(mass),
	m_invMass(1/mass),
	m_acceleration(new Vector2())
{

}

RigidBodyComponent::~RigidBodyComponent()
{
	delete m_acceleration;
	m_forces.clear();
}

void RigidBodyComponent::Update(float delta)
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
	
	*m_acceleration = forcesSum / m_mass;
	*m_entity->GetVelocity() += *m_acceleration * delta;

	float speedLengthSq = m_entity->GetVelocity()->LengthSq();
	if (speedLengthSq < Vector2::EPSILON_SQ)
	{
		m_entity->GetVelocity()->x = 0;
		m_entity->GetVelocity()->y = 0;
	}
	else
	{
		*m_entity->GetPosition() += *m_entity->GetVelocity() * delta;
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

Type RigidBodyComponent::GetType() const
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


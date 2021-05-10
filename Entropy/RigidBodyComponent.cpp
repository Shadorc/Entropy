#include "Entity.h"
#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(const Entity* entity, float mass) :
	Component(entity),
	m_mass(mass),
	m_acceleration(new Vector2())
{

}

RigidBodyComponent::~RigidBodyComponent()
{
	delete m_acceleration;
	for (Vector2* force : m_forces)
	{
		delete force;
	}
	m_forces.clear();
}

void RigidBodyComponent::Update(float delta)
{
	Vector2 forcesSum;
	for (const Vector2* force : m_forces) 
	{
		forcesSum += *force;
	}
	*m_acceleration = forcesSum / m_mass;
	*m_entity->GetVelocity() += *m_acceleration * delta;

	float speedLengthSq = m_entity->GetVelocity()->LengthSq();
	if (speedLengthSq < Vector2::EPSILON_SQ)
	{
		m_entity->GetVelocity()->m_x = 0;
		m_entity->GetVelocity()->m_y = 0;
	}
	else
	{
		*m_entity->GetPosition() += *m_entity->GetVelocity() * delta;
	}
}

void RigidBodyComponent::AddForce(Vector2* force)
{
	m_forces.push_back(force);
}

float RigidBodyComponent::GetMass() const
{
	return m_mass;
}


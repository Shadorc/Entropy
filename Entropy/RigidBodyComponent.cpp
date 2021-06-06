#include "Precompiled.h"

constexpr float MASS_METER_SQUARE = 1.0f / 1000.0f; // kg.m-2

RigidBodyComponent::RigidBodyComponent(Entity* entity)
	: RigidBodyComponent(entity, RigidbodyType::STATIC)
{

}

RigidBodyComponent::RigidBodyComponent(Entity* entity, RigidbodyType type)
	: Component(entity)
	, m_type(type)
	, m_massData()
	, m_material({ 1.0f, 0.2f })
	, m_acceleration()
{
	ComputeMass();
}

RigidBodyComponent::~RigidBodyComponent()
{
	m_forces.clear();
}

void RigidBodyComponent::ComputeMass()
{
	switch (m_entity->GetType())
	{
	case EntityType::CIRCLE:
	{
		const entity::Circle* circle = dynamic_cast<entity::Circle*>(m_entity);
		const float radiusSq = circle->GetRadius() * circle->GetRadius();
		m_massData.mass = PI * radiusSq * m_material.density * MASS_METER_SQUARE;
		m_massData.invMass = (m_massData.mass > 0) ? (1.0f / m_massData.mass) : 0.0f;
		m_massData.inertia = m_massData.mass * radiusSq;
		m_massData.invInertia = (m_massData.inertia > 0) ? (1.0f / m_massData.inertia) : 0.0f;
		break;
	}
	case EntityType::RECTANGLE:
	{
		const entity::Rectangle* rectangle = dynamic_cast<entity::Rectangle*>(m_entity);
		m_massData.mass = rectangle->GetWidth() * rectangle->GetHeight() * m_material.density * MASS_METER_SQUARE;
		m_massData.invMass = (m_massData.mass > 0) ? (1.0f / m_massData.mass) : 0.0f;
		m_massData.inertia = rectangle->GetWidth() * FLOAT(pow(rectangle->GetHeight(), 3.0f)) / 12; // TODO
		m_massData.invInertia = (m_massData.inertia > 0) ? (1.0f / m_massData.inertia) : 0.0f;
		break;
	}
	}
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
	
	m_acceleration = forcesSum * m_massData.invMass;
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

MassData RigidBodyComponent::GetMassData() const
{
	return m_massData;
}

Material RigidBodyComponent::GetMaterial() const
{
	return m_material;
}

float RigidBodyComponent::GetRestitution() const
{
	return 0.2f;
}

bool RigidBodyComponent::IsStatic() const
{
	return m_type == RigidbodyType::STATIC;
}


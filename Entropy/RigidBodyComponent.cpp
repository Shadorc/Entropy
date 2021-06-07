#include "Precompiled.h"

constexpr float MASS_METER_SQUARE = 1.0f / 1000.0f; // kg.m-2

RigidbodyComponent::RigidbodyComponent(Entity* entity, Material material)
	: Component(entity)
	, m_massData()
	, m_material(material)
	, m_frictionData({ 0.1f, 0.05f })
{
	ComputeMass();
}

void RigidbodyComponent::ComputeMass()
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

void RigidbodyComponent::Update(float deltaTime)
{
	Vector2 forcesSum;
	for (Vector2& force : m_forces)
	{
		forcesSum += force;
	}
	
	m_entity->velocity += forcesSum * m_massData.invMass * deltaTime;
	m_entity->position += m_entity->velocity * deltaTime;

	m_forces.clear();
}

void RigidbodyComponent::AddForce(Vector2 force)
{
	m_forces.emplace_back(force);
}

MassData RigidbodyComponent::GetMassData() const
{
	return m_massData;
}

Material RigidbodyComponent::GetMaterial() const
{
	return m_material;
}

FrictionData RigidbodyComponent::GetFrictionData() const
{
	return m_frictionData;
}


#include "Precompiled.h"

constexpr float MASS_METER_SQUARE = 1.0f / 1000.0f; // kg.m-2

RigidbodyComponent::RigidbodyComponent(Entity* entity, MaterialData materialData)
	: Component(entity)
	, m_massData()
	, m_materialData(materialData)
	, m_frictionData({ 0.2f, 0.1f })
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
		m_massData.mass = PI * radiusSq * m_materialData.density * MASS_METER_SQUARE;
		m_massData.invMass = (m_massData.mass > 0) ? (1.0f / m_massData.mass) : 0.0f;
		m_massData.inertia = m_massData.mass * radiusSq;
		m_massData.invInertia = (m_massData.inertia > 0) ? (1.0f / m_massData.inertia) : 0.0f;
		break;
	}
	case EntityType::POLYGON:
	{
		const entity::Polygon* polygon = dynamic_cast<entity::Polygon*>(m_entity);

		// Calculate centroid and moment of interia
		Vector2 centroid;
		float area = 0.0f;
		float inertia = 0.0f;

		for (size_t vertexIdx1 = 0; vertexIdx1 < polygon->GetVertexCount(); ++vertexIdx1)
		{
			// Triangle vertices, third vertex implied as (0, 0)
			const Vector2& vertex1 = polygon->GetVertex(vertexIdx1);
			size_t vertexIdx2 = (vertexIdx1 + 1) % polygon->GetVertexCount();
			const Vector2& vertex2 = polygon->GetVertex(vertexIdx2);

			float z = vertex1.Cross(vertex2);
			float triangleArea = z / 2.0f;

			area += triangleArea;

			// Use area to weight the centroid average, not just vertex position
			centroid += triangleArea * (vertex1 + vertex2) / 3.0f;

			float intx2 = vertex1.x * vertex1.x + vertex2.x * vertex1.x + vertex2.x * vertex2.x;
			float inty2 = vertex1.y * vertex1.y + vertex2.y * vertex1.y + vertex2.y * vertex2.y;
			inertia += z * (intx2 + inty2) / 12.0f;
		}

		centroid /= area;

		m_massData.mass = m_materialData.density * area * MASS_METER_SQUARE;
		m_massData.invMass = (m_massData.mass > 0.0f) ? (1.0f / m_massData.mass) : 0.0f;
		m_massData.inertia = inertia * m_materialData.density * MASS_METER_SQUARE;
		m_massData.invInertia = (m_massData.inertia > 0.0f) ? (1.0f / m_massData.inertia) : 0.0f;
		break;
	}
	default:
		ENTROPY_ASSERT(false, "Unknown entity type");
	}
}

void RigidbodyComponent::Update(float deltaTime)
{
	if (IsZero(m_massData.invMass))
	{
		return;
	}

	m_entity->velocity += m_entity->force * m_massData.invMass * deltaTime;
	m_entity->position += m_entity->velocity * deltaTime;

	m_entity->angularVelocity += m_entity->torque * m_massData.invInertia * deltaTime;
	m_entity->Rotate(m_entity->angularVelocity * deltaTime);

	m_entity->force.Reset();
	m_entity->torque = 0.0f;
}

void RigidbodyComponent::AddForce(const Vector2& force)
{
	m_entity->force += force;
}

MassData RigidbodyComponent::GetMassData() const
{
	return m_massData;
}

MaterialData RigidbodyComponent::GetMaterialData() const
{
	return m_materialData;
}

FrictionData RigidbodyComponent::GetFrictionData() const
{
	return m_frictionData;
}


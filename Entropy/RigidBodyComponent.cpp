#include "Precompiled.h"

constexpr float MASS_METER_SQUARE = 1.0f / 1000.0f; // kg.m-2

RigidbodyComponent::RigidbodyComponent(Entity* entity)
	: RigidbodyComponent(entity, MATERIAL_ROCK, FRICTION_NORMAL)
{

}

RigidbodyComponent::RigidbodyComponent(Entity* entity, MaterialData materialData, FrictionData frictionData)
	: Component(entity)
	, m_MassData()
	, m_MaterialData(materialData)
	, m_FrictionData(frictionData)
	, m_Force()
	, m_Torque(0.0f)
{
	ComputeMass();
}

void RigidbodyComponent::ComputeMass()
{
	switch (m_Entity->GetType())
	{
	case EntityType::CIRCLE:
	{
		const entity::Circle* circle = dynamic_cast<entity::Circle*>(m_Entity);
		ENTROPY_ASSERT(circle);

		const float radiusSq = circle->GetRadius() * circle->GetRadius();
		m_MassData.mass = PI * radiusSq * m_MaterialData.density * MASS_METER_SQUARE;
		m_MassData.invMass = (m_MassData.mass > 0) ? (1.0f / m_MassData.mass) : 0.0f;
		m_MassData.inertia = m_MassData.mass * radiusSq;
		m_MassData.invInertia = (m_MassData.inertia > 0) ? (1.0f / m_MassData.inertia) : 0.0f;
		break;
	}
	case EntityType::POLYGON:
	{
		const entity::Polygon* polygon = dynamic_cast<entity::Polygon*>(m_Entity);
		ENTROPY_ASSERT(polygon);

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

			const float z = vertex1.Cross(vertex2);
			const float triangleArea = z / 2.0f;

			area += triangleArea;

			// Use area to weight the centroid average, not just vertex position
			centroid += triangleArea * (vertex1 + vertex2) / 3.0f;

			const float intx2 = vertex1.x * vertex1.x + vertex2.x * vertex1.x + vertex2.x * vertex2.x;
			const float inty2 = vertex1.y * vertex1.y + vertex2.y * vertex1.y + vertex2.y * vertex2.y;
			inertia += z * (intx2 + inty2) / 12.0f;
		}

		centroid /= area;

		m_MassData.mass = m_MaterialData.density * area * MASS_METER_SQUARE;
		m_MassData.invMass = (m_MassData.mass > 0.0f) ? (1.0f / m_MassData.mass) : 0.0f;
		m_MassData.inertia = inertia * m_MaterialData.density * MASS_METER_SQUARE;
		m_MassData.invInertia = (m_MassData.inertia > 0.0f) ? (1.0f / m_MassData.inertia) : 0.0f;
		break;
	}
	default:
		ENTROPY_ASSERT_WITH_REASON(false, "Unknown entity type");
	}
}

void RigidbodyComponent::Update(float deltaTime)
{
	if (IsZero(m_MassData.invMass))
	{
		return;
	}

	m_Entity->velocity += m_Force * m_MassData.invMass * deltaTime;
	m_Entity->Translate(m_Entity->velocity * deltaTime);

	m_Entity->angularVelocity += m_Torque * m_MassData.invInertia * deltaTime;
	m_Entity->Rotate(m_Entity->angularVelocity * deltaTime);

	m_Force.Reset();
	m_Torque = 0.0f;
}

void RigidbodyComponent::ApplyImpulse(const Vector2& impulse, const Vector2& contactVector)
{
	m_Entity->velocity += m_MassData.invMass * impulse;
	m_Entity->angularVelocity += m_MassData.invInertia * contactVector.Cross(impulse);
}

void RigidbodyComponent::AddForce(const Vector2& force)
{
	m_Force += force;
}

void RigidbodyComponent::AddTorque(float torque)
{
	m_Torque += torque;
}

MassData RigidbodyComponent::GetMassData() const
{
	return m_MassData;
}

MaterialData RigidbodyComponent::GetMaterialData() const
{
	return m_MaterialData;
}

FrictionData RigidbodyComponent::GetFrictionData() const
{
	return m_FrictionData;
}


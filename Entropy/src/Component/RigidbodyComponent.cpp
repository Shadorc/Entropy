#include "Precompiled.h"

constexpr float MASS_METER_SQUARE = 1.0f / 1000.0f; // kg.m-2

RigidbodyComponent::RigidbodyComponent(const std::weak_ptr<Entity>& entity)
	: RigidbodyComponent(entity, MATERIAL_ROCK, FRICTION_NORMAL)
{

}

RigidbodyComponent::RigidbodyComponent(const std::weak_ptr<Entity>& entity, MaterialData materialData, FrictionData frictionData)
	: Component(entity)
	, m_MaterialData(materialData)
	, m_FrictionData(frictionData)
	, m_MassData(ComputeMass())
	, m_Force()
	, m_Torque(0.0f)
{

}

MassData RigidbodyComponent::ComputeMass()
{
	MassData massData = {};

	const auto& entity = m_Entity.lock();
	if (entity)
	{
		switch (entity->GetType())
		{
		case EntityType::CIRCLE:
		{
			const entity::Circle* circle = dynamic_cast<entity::Circle*>(entity.get());
			ENTROPY_ASSERT(circle);

			const float radiusSq = circle->GetRadius() * circle->GetRadius();

			massData.mass = PI * radiusSq * m_MaterialData.density * MASS_METER_SQUARE;
			massData.invMass = (massData.mass > 0) ? (1.0f / massData.mass) : 0.0f;
			massData.inertia = massData.mass * radiusSq;
			massData.invInertia = (massData.inertia > 0) ? (1.0f / massData.inertia) : 0.0f;
			break;
		}
		case EntityType::POLYGON:
		{
			const entity::Polygon* polygon = dynamic_cast<entity::Polygon*>(entity.get());
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

			massData.mass = m_MaterialData.density * area * MASS_METER_SQUARE;
			massData.invMass = (massData.mass > 0.0f) ? (1.0f / massData.mass) : 0.0f;
			massData.inertia = inertia * m_MaterialData.density * MASS_METER_SQUARE;
			massData.invInertia = (massData.inertia > 0.0f) ? (1.0f / massData.inertia) : 0.0f;
			break;
		}
		default:
			ENTROPY_ASSERT_WITH_REASON(false, "Unknown entity type");
		}
	}
	return massData;
}

void RigidbodyComponent::Update(float deltaTime)
{
	if (IsZero(m_MassData.invMass))
	{
		return;
	}

	const auto& entity = m_Entity.lock();
	if (entity)
	{
		entity->velocity += m_Force * m_MassData.invMass * deltaTime;
		entity->Translate(entity->velocity * deltaTime);

		entity->angularVelocity += m_Torque * m_MassData.invInertia * deltaTime;
		entity->Rotate(entity->angularVelocity * deltaTime);
	}

	m_Force.Reset();
	m_Torque = 0.0f;
}

void RigidbodyComponent::ApplyImpulse(const Vector2& impulse, const Vector2& contactVector) const
{
	const auto& entity = m_Entity.lock();
	if (entity)
	{
		entity->velocity += m_MassData.invMass * impulse;
		entity->angularVelocity += m_MassData.invInertia * contactVector.Cross(impulse);
	}
}

void RigidbodyComponent::AddForce(const Vector2& force)
{
	m_Force += force;
}

void RigidbodyComponent::AddTorque(float torque)
{
	m_Torque += torque;
}

const MassData& RigidbodyComponent::GetMassData() const
{
	return m_MassData;
}

const MaterialData& RigidbodyComponent::GetMaterialData() const
{
	return m_MaterialData;
}

const FrictionData& RigidbodyComponent::GetFrictionData() const
{
	return m_FrictionData;
}


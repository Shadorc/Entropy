#include "Precompiled.h"

CollisionManager::CollisionManager()
	: m_QuadTree(AABB(0.0f, 0.0f, static_cast<float>(WIDTH), static_cast<float>(HEIGHT)))
{

}

void CollisionManager::Update()
{
	const auto& entities = Sandbox::INSTANCE->GetEntities();
	// Push all entities with a rigidbody inside m_Entities
	m_Entities.clear();
	m_Entities.reserve(entities.size());
	for (const auto& entity : entities)
	{
		if (entity->HasComponent<RigidbodyComponent>())
		{
			m_Entities.push_back(entity.get());
		}
	}

	UpdateQuadTree();
	BroadPhase();
	for (int i = 0; i < COLLISIONS_SOLVER_ITERATIONS; ++i)
	{
		SolveCollisions();
	}
}

void CollisionManager::UpdateQuadTree()
{
	ENTROPY_PROFILE_FUNCTION();

	m_QuadTree.Clear();
	for (Entity* entity : m_Entities)
	{
		m_QuadTree.Insert(entity);
	}
}

void CollisionManager::BroadPhase()
{
	ENTROPY_PROFILE_FUNCTION();

	m_Pairs.clear();

	// Detect all colliding entities and push the corresponding pair inside m_pairs
	for (Entity* entityA : m_Entities)
	{
		for (Entity* entityB : m_QuadTree.Search(entityA))
		{
			if (entityA == entityB)
			{
				continue;
			}

			// Do not check collisions between static objects
			if (IsZero(entityA->GetComponent<RigidbodyComponent>().GetMassData().mass)
				&& IsZero(entityB->GetComponent<RigidbodyComponent>().GetMassData().mass))
			{
				continue;
			}

			if (entityA->GetAABB().IntersectsWith(entityB->GetAABB()))
			{
				m_Pairs.emplace_back(entityA, entityB);
			}
		}
	}
}

void CollisionManager::SolveCollisions()
{
	ENTROPY_PROFILE_FUNCTION();

	for (const std::tuple<Entity*, Entity*>& pair : m_Pairs)
	{
		Collision collision(std::get<0>(pair), std::get<1>(pair));
		Solve(collision);
		if (!collision.contacts.empty())
		{
			ApplyImpulses(collision);
			CorrectPosition(collision);
		}
	}
}

void CollisionManager::ApplyImpulses(const Collision& collision)
{
	Entity* entityA = collision.entityA;
	Entity* entityB = collision.entityB;

	const auto& rigidbodyA = entityA->GetComponent<RigidbodyComponent>();
	const auto& rigidbodyB = entityB->GetComponent<RigidbodyComponent>();
	const MassData& massA = rigidbodyA.GetMassData();
	const MassData& massB = rigidbodyB.GetMassData();

	const float staticFriction = sqrtf(rigidbodyA.GetFrictionData().staticFactor * rigidbodyB.GetFrictionData().staticFactor);
	const float dynamicFriction = sqrtf(rigidbodyA.GetFrictionData().dynamicFactor * rigidbodyB.GetFrictionData().dynamicFactor);

	float restitution = sqrtf(rigidbodyA.GetMaterialData().restitution * rigidbodyB.GetMaterialData().restitution);
	for (const Vector2& contact : collision.contacts)
	{
		// Calculate radii from COM to contact
		const Vector2& radiusA = contact - entityA->GetPosition();
		const Vector2& radiusB = contact - entityB->GetPosition();

		const Vector2& relativeVelocity =
			entityB->velocity + Vector2::Cross(entityB->angularVelocity, radiusB)
			- (entityA->velocity + Vector2::Cross(entityA->angularVelocity, radiusA));

		// Determine if we should perform a resting collision or not
		// The idea is if the only thing moving this object is gravity,
		// then the collision should be performed without any restitution
		if (Equal(relativeVelocity.y, DELTA_TIME * GravityComponent::GRAVITY.y))
		{
			restitution = 0.0f;
			break;
		}
	}

	for (const Vector2& contact : collision.contacts)
	{
		const Vector2& radiusA = contact - entityA->GetPosition();
		const Vector2& radiusB = contact - entityB->GetPosition();

		Vector2 relativeVelocity =
			entityB->velocity + Vector2::Cross(entityB->angularVelocity, radiusB)
			- (entityA->velocity + Vector2::Cross(entityA->angularVelocity, radiusA));
		const float velAlongNormal = relativeVelocity.Dot(collision.normal);

		// Do not resolve if velocities are separating
		if (velAlongNormal > 0)
		{
			return;
		}

		float radiusAcrossNormal = radiusA.Cross(collision.normal);
		float radiusBcrossNormal = radiusB.Cross(collision.normal);
		float invMassSum = massA.invMass + massB.invMass
			+ radiusAcrossNormal * radiusAcrossNormal * massA.invInertia
			+ radiusBcrossNormal * radiusBcrossNormal * massB.invInertia;

		float normalImpulseScalar = -(1.0f + restitution) * velAlongNormal / (invMassSum * collision.contacts.size());
		if (!IsZero(normalImpulseScalar))
		{
			// Apply normal impulse
			const Vector2& normalImpulse = collision.normal * normalImpulseScalar;
			rigidbodyA.ApplyImpulse(-normalImpulse, radiusA);
			rigidbodyB.ApplyImpulse(normalImpulse, radiusB);
		}

		// Update relative velocity
		relativeVelocity =
			entityB->velocity + Vector2::Cross(entityB->angularVelocity, radiusB)
			- (entityA->velocity + Vector2::Cross(entityA->angularVelocity, radiusA));

		Vector2 tangent = relativeVelocity - (collision.normal * relativeVelocity.Dot(collision.normal));
		const float tangentLen = tangent.Length();
		if (!IsZero(tangentLen))
		{
			// Normalize the tangent with its pre-calculated length
			tangent /= tangentLen;

			const float frictionImpulseScalar = -relativeVelocity.Dot(tangent) / (invMassSum * collision.contacts.size());

			// Clamp magnitude of friction and create friction impulse vector
			Vector2 frictionImpulse;
			if (abs(frictionImpulseScalar) < normalImpulseScalar * staticFriction)
			{
				frictionImpulse = tangent * frictionImpulseScalar;
			}
			else
			{
				frictionImpulse = tangent * -normalImpulseScalar * dynamicFriction;
			}

			// Apply friction impulse
			rigidbodyA.ApplyImpulse(-frictionImpulse, radiusA);
			rigidbodyB.ApplyImpulse(frictionImpulse, radiusB);
		}
	}
}

void CollisionManager::CorrectPosition(const Collision& collision)
{
	const float invMassA = collision.entityA->GetComponent<RigidbodyComponent>().GetMassData().invMass;
	const float invMassB = collision.entityB->GetComponent<RigidbodyComponent>().GetMassData().invMass;
	const Vector2& correction = (std::max(collision.penetration - PENETRATION_ALLOWANCE, 0.0f) / (invMassA + invMassB)) * collision.normal * PENETRATION_PERCENT;
	if (!IsZero(invMassA))
	{
		collision.entityA->Translate(-correction * invMassA);
	}
	if (!IsZero(invMassB))
	{
		collision.entityB->Translate(correction * invMassB);
	}
}

const QuadTree<Entity>& CollisionManager::GetRootQuadTree() const
{
	return m_QuadTree;
}


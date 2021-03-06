#include "Precompiled.h"

CollisionManager::CollisionManager(const Sandbox* sandbox)
	: m_Sandbox(sandbox)
	, m_QuadTree(nullptr)
{
	SetRootSize(WIDTH, HEIGHT);
}

CollisionManager::~CollisionManager()
{
	ENTROPY_DELETE(m_QuadTree);
}

void CollisionManager::Update()
{
	// Push all entities with a rigid body inside m_entities
	m_Entities.clear();
	for (Entity* entity : m_Sandbox->GetEntities())
	{
		if (entity->GetRigidbodyComponent() != nullptr)
		{
			m_Entities.push_back(entity);
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
	m_QuadTree->Clear();
	for (Entity* entity : m_Entities)
	{
		m_QuadTree->Insert(entity);
	}
}

void CollisionManager::BroadPhase()
{
	m_Pairs.clear();
	m_UniquePairs.clear();

	// Detect all colliding entities and push the corresponding pair inside m_pairs
	for (Entity* entityA : m_Entities)
	{
		for (Entity* entityB : m_QuadTree->Search(entityA))
		{
			if (entityA == entityB)
			{
				continue;
			}

			// Do not check collisions between static objects
			if (IsZero(entityA->GetRigidbodyComponent()->GetMassData().mass)
				&& IsZero(entityB->GetRigidbodyComponent()->GetMassData().mass))
			{
				continue;
			}

			if (entityA->GetAABB()->IntersectsWith(entityB->GetAABB()))
			{
				m_Pairs.emplace_back(entityA, entityB);
			}
		}
	}

	// Sort the m_pairs vector so that all similar pairs follow each other    
	std::sort(m_Pairs.begin(), m_Pairs.end(), PairComparator<Entity>);

	// Push non-duplicate pairs inside m_uniquePairs
	int i = 0;
	while (i < m_Pairs.size())
	{
		const Pair<Entity>& pair = m_Pairs[i];
		m_UniquePairs.push_back(pair);

		++i;

		while (i < m_Pairs.size())
		{
			const Pair<Entity>& otherPair = m_Pairs[i];
			if (pair.left == otherPair.right && pair.right == otherPair.left)
			{
				++i;
			}
			else
			{
				break;
			}
		}
	}
}

void CollisionManager::SolveCollisions()
{
	for (Pair<Entity>& pair : m_UniquePairs)
	{
		const Collision& collision = Solve(pair.left, pair.right);
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

	const auto& rigidbodyA = entityA->GetRigidbodyComponent();
	const auto& rigidbodyB = entityB->GetRigidbodyComponent();
	const MassData& massA = rigidbodyA->GetMassData();
	const MassData& massB = rigidbodyB->GetMassData();

	const float staticFriction = sqrtf(rigidbodyA->GetFrictionData().staticFactor * rigidbodyB->GetFrictionData().staticFactor);
	const float dynamicFriction = sqrtf(rigidbodyA->GetFrictionData().dynamicFactor * rigidbodyB->GetFrictionData().dynamicFactor);

	float restitution = sqrtf(rigidbodyA->GetMaterialData().restitution * rigidbodyB->GetMaterialData().restitution);
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
			rigidbodyA->ApplyImpulse(-normalImpulse, radiusA);
			rigidbodyB->ApplyImpulse(normalImpulse, radiusB);
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
			rigidbodyA->ApplyImpulse(-frictionImpulse, radiusA);
			rigidbodyB->ApplyImpulse(frictionImpulse, radiusB);
		}
	}
}

void CollisionManager::CorrectPosition(const Collision& collision)
{
	const float invMassA = collision.entityA->GetRigidbodyComponent()->GetMassData().invMass;
	const float invMassB = collision.entityB->GetRigidbodyComponent()->GetMassData().invMass;
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

const QuadTree<Entity>* CollisionManager::GetRootQuadTree() const
{
	return m_QuadTree;
}

void CollisionManager::SetRootSize(int width, int height)
{
	ENTROPY_DELETE(m_QuadTree);
	m_QuadTree = ENTROPY_NEW(QuadTree<Entity>, ENTROPY_NEW(AABB, 0.0f, 0.0f, (float)width, (float)height));
}

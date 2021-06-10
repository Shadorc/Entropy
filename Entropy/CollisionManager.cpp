#include "Precompiled.h"

CollisionManager::CollisionManager(const Sandbox* sandbox) 
    : m_sandbox(sandbox)
    , m_quadTree(nullptr)
{
    SetRootSize(WIDTH, HEIGHT);
}

CollisionManager::~CollisionManager()
{
    delete m_quadTree;
}

void CollisionManager::Update()
{
    // Push all entities with a rigid body inside m_entities
    m_entities.clear();
    for (Entity* entity : m_sandbox->GetEntities())
    {
        if (entity->GetRigidbodyComponent() != nullptr)
        {
            m_entities.emplace_back(entity);
        }
    }

    UpdateQuadTree();
    BroadPhase();
    SolveCollisions();
}

void CollisionManager::UpdateQuadTree()
{
	m_quadTree->Clear();
	for (Entity* entity : m_entities)
	{
	    m_quadTree->Insert(entity);
	}
}

void CollisionManager::BroadPhase()
{
    m_pairs.clear(); 
    m_uniquePairs.clear();

    // Detect all colliding entities and push the corresponding pair inside m_pairs
    for (Entity* entityA : m_entities)
    {
        for (Entity* entityB : m_quadTree->Search(entityA))
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

            if (entityA->GetAABB().IntersectsWith(entityB->GetAABB()))
            {
                m_pairs.emplace_back(entityA, entityB);
            }
        }
    }

    // Sort the m_pairs vector so that all similar pairs follow each other    
    std::sort(m_pairs.begin(), m_pairs.end(), PairComparator<Entity>);

    // Push non-duplicate pairs inside m_uniquePairs
    int i = 0;
    while (i < m_pairs.size())
    {
        const Pair<Entity>& pair = m_pairs[i];
        m_uniquePairs.emplace_back(pair);

        ++i;

        while (i < m_pairs.size())
        {
            const Pair<Entity>& otherPair = m_pairs[i];
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
    for (Pair<Entity>& pair : m_uniquePairs)
    {
        const Collision& manifold = Solve(pair.left, pair.right);
        if (!IsZero(manifold.penetration))
        {
            ApplyImpulses(manifold);
            CorrectPosition(manifold);
        }
    }
}

void CollisionManager::ApplyImpulses(const Collision& manifold)
{
    Entity* entityA = manifold.entityA;
    Entity* entityB = manifold.entityB;

    const Vector2& relativVelocity = entityB->velocity - entityA->velocity;
    const float velAlongNormal = relativVelocity.Dot(manifold.normal);

    // Do not resolve if velocities are separating
    if (velAlongNormal > 0)
    {
        return;
    }

    const auto& rigidbodyA = entityA->GetRigidbodyComponent();
    const auto& rigidbodyB = entityB->GetRigidbodyComponent();
    const MassData& massA = rigidbodyA->GetMassData();
    const MassData& massB = rigidbodyB->GetMassData();

    // Calculate restitution impulse
    float restitution = sqrtf(rigidbodyA->GetMaterial().restitution * rigidbodyB->GetMaterial().restitution);
    
    // Determine if we should perform a resting collision or not
    // The idea is if the only thing moving this object is gravity, then the collision should be performed without any restitution
    // TODO: Fix
    if (Equal(relativVelocity.y, GravityComponent::GRAVITY.y * DELTA_TIME))
    {
        restitution = 0.0f;
    }

    const float normalImpulseScalar = -(1.0f + restitution) * velAlongNormal / (massA.invMass + massB.invMass);
    if (!IsZero(normalImpulseScalar))
    {
        const Vector2& impulse = manifold.normal * normalImpulseScalar;

        //Apply impulse
        const float massSum = massA.mass + massB.mass;
        const float ratioA = massA.mass / massSum;
        entityA->velocity -= impulse * ratioA;
        const float ratioB = massB.mass / massSum;
        entityB->velocity += impulse * ratioB;
    }

    // Calculate friction vector
    Vector2 tangent = relativVelocity - relativVelocity.Dot(manifold.normal) * manifold.normal;
    const float tangentLen = tangent.Length();
    if (!IsZero(tangentLen))
    {
        // Normalize tangent with the already calculated length
        tangent /= tangentLen;

        const float frictionImpulseScalar = -relativVelocity.Dot(tangent) / (massA.invMass + massB.invMass);

        const float staticFriction = sqrtf(rigidbodyA->GetFrictionData().staticFactor * rigidbodyB->GetFrictionData().staticFactor);
        const float dynamicFriction = sqrtf(rigidbodyA->GetFrictionData().dynamicFactor * rigidbodyB->GetFrictionData().dynamicFactor);
            
        // Clamp magnitude of friction and create friction impulse vector
        Vector2 friction;
        if (abs(frictionImpulseScalar) < normalImpulseScalar * staticFriction)
        {
            friction = frictionImpulseScalar * tangent;
        }
        else
        {
            friction = -normalImpulseScalar * tangent * dynamicFriction;
        }

        // Apply friction
        entityA->velocity -= massA.invMass * friction;
        entityB->velocity += massB.invMass * friction;
    }
}

void CollisionManager::CorrectPosition(const Collision& manifold)
{
    const float invMassA = manifold.entityA->GetRigidbodyComponent()->GetMassData().invMass;
    const float invMassB = manifold.entityB->GetRigidbodyComponent()->GetMassData().invMass;
    const Vector2& correction = (std::max(manifold.penetration - PENETRATION_ALLOWANCE, 0.0f) / (invMassA + invMassB)) * manifold.normal * PENETRATION_PERCENT;
    manifold.entityA->position -= correction * invMassA;
    manifold.entityB->position += correction * invMassB;
}

const QuadTree<Entity>* CollisionManager::GetRootQuadTree() const
{
    return m_quadTree;
}

void CollisionManager::SetRootSize(int width, int height)
{
    delete m_quadTree;
	m_quadTree = new QuadTree<Entity>(AABB(Vector2(), Vector2((float) width, (float) height)));
}

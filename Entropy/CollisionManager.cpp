#include <algorithm>

#include "Precompiled.h"

constexpr float PENETRATION_PERCENT = 0.4f; // Penetration percentage to correct
constexpr float PENETRATION_ALLOWANCE = 0.05f; // Penetration allowance

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

const QuadTree<Entity>* CollisionManager::GetRootQuadTree() const
{
    return m_quadTree;
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
    CheckCollisions();
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
            const Pair<Entity>& potentialDup = m_pairs[i];
            if (pair.left == potentialDup.right && pair.right == potentialDup.left)
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

void CollisionManager::CheckCollisions()
{
    for (Pair<Entity>& pair : m_uniquePairs)
    {
        // Do not check collisions between static objects
        if (IS_ZERO(pair.left->GetRigidbodyComponent()->GetMassData().mass) 
            && IS_ZERO(pair.right->GetRigidbodyComponent()->GetMassData().mass))
        {
            continue;
        }

        const Collision& manifold = Solve(pair.left, pair.right);
        if (!IS_ZERO(manifold.penetration))
        {
            ResolveCollision(manifold);
            PositionalCorrection(manifold);
        }
    }
}

void CollisionManager::ResolveCollision(const Collision& manifold)
{
    Entity* entityA = manifold.entityA;
    Entity* entityB = manifold.entityB;

    // Calculate relative velocity
    const Vector2& relativVelocity = entityB->velocity - entityA->velocity;

    // Calculate relative velocity in terms of the normal direction
    const float velAlongNormal = relativVelocity.Dot(manifold.normal);

    // Do not resolve if velocities are separating
    if (velAlongNormal > 0)
    {
        return;
    }

    const RigidbodyComponent* rigidbodyA = entityA->GetRigidbodyComponent();
    const RigidbodyComponent* rigidbodyB = entityB->GetRigidbodyComponent();
    const MassData& massA = rigidbodyA->GetMassData();
    const MassData& massB = rigidbodyB->GetMassData();

    // Calculate impulse
    const float restitution = MIN(rigidbodyA->GetMaterial().restitution, rigidbodyB->GetMaterial().restitution);
    const float normalImpulseScalar = -(1 + restitution) * velAlongNormal / (massA.invMass + massB.invMass);
    if (!IS_ZERO(normalImpulseScalar))
    {
        const Vector2& impulse = manifold.normal * normalImpulseScalar;

        //Apply impulse
        const float massSum = massA.mass + massB.mass;
        const float ratioA = massA.mass / massSum;
        entityA->velocity -= impulse * ratioA;
        const float ratioB = massB.mass / massSum;
        entityB->velocity += impulse * ratioB;
    }

    // Compute friction factors
    const float staticFriction = PYTHAGORE(rigidbodyA->GetFrictionData().staticFactor, rigidbodyB->GetFrictionData().staticFactor);
    const float dynamicFriction = PYTHAGORE(rigidbodyA->GetFrictionData().dynamicFactor, rigidbodyB->GetFrictionData().dynamicFactor);

    // Calculate friction
    Vector2 tangent = relativVelocity - relativVelocity.Dot(manifold.normal) * manifold.normal;
    tangent.Normalize();

    const float frictionImpulseScalar = -relativVelocity.Dot(tangent) / (massA.invMass + massB.invMass);
    if (!IS_ZERO(frictionImpulseScalar))
    {
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

void CollisionManager::PositionalCorrection(const Collision& manifold)
{
    const RigidbodyComponent* rigidbodyA = manifold.entityA->GetRigidbodyComponent();
    const RigidbodyComponent* rigidbodyB = manifold.entityB->GetRigidbodyComponent();
    float invMassA = rigidbodyA->GetMassData().invMass;
    float invMassB = rigidbodyB->GetMassData().invMass;
    const Vector2& correction = (MAX(manifold.penetration - PENETRATION_ALLOWANCE, 0.0f) / (invMassA + invMassB)) * manifold.normal * PENETRATION_PERCENT;
    manifold.entityA->position -= correction * invMassA;
    manifold.entityB->position += correction * invMassB;
}

void CollisionManager::SetRootSize(int width, int height)
{
    delete m_quadTree;
	m_quadTree = new QuadTree<Entity>(AABB(Vector2(), Vector2(FLOAT(width), FLOAT(height))));
}

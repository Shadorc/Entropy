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

const QuadTree<Entity>* CollisionManager::GetRootQuadTree() const
{
    return m_quadTree;
}

void CollisionManager::Update(float deltaTime)
{
	UpdateQuadTree();
	CheckCollisions(deltaTime);
}

void CollisionManager::UpdateQuadTree()
{
	m_quadTree->Clear();
	for (Entity* object : m_sandbox->GetEntities())
	{
        if (object->GetRigidBodyComponent() != nullptr)
        {
	        m_quadTree->Insert(object);
        }
	}
}

void ResolveCollision(const Collision& manifold)
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

    const RigidBodyComponent* rigidbodyA = entityA->GetRigidBodyComponent();
    const RigidBodyComponent* rigidbodyB = entityB->GetRigidBodyComponent();
    const MassData& massA = rigidbodyA->GetMassData();
    const MassData& massB = rigidbodyB->GetMassData();

    // Calculate impulse
    const float restitution = MIN(rigidbodyA->GetMaterial().restitution, rigidbodyB->GetMaterial().restitution);
    const float normalImpulseScalar = -(1 + restitution) * velAlongNormal / (massA.invMass + massB.invMass);
    const Vector2& impulse = manifold.normal * normalImpulseScalar;

    //Apply impule
    const float massSum = massA.mass + massB.mass;
    if (!rigidbodyA->IsStatic())
    {
        const float ratioA = massA.mass / massSum;
        entityA->velocity -= impulse * ratioA;
    }
    if (!rigidbodyB->IsStatic())
    {
        const float ratioB = massB.mass / massSum;
        entityB->velocity += impulse * ratioB;
    }

    const float staticFriction = PYTHAGORE(rigidbodyA->GetFrictionData().staticFactor, rigidbodyB->GetFrictionData().staticFactor);
    const float dynamicFriction = PYTHAGORE(rigidbodyA->GetFrictionData().dynamicFactor, rigidbodyB->GetFrictionData().dynamicFactor);

    // Calculate tangent friction
    Vector2 tangent = relativVelocity - relativVelocity.Dot(manifold.normal) * manifold.normal;
    tangent.Normalize();

    const float frictionImpulseScalar = -relativVelocity.Dot(tangent) / (massA.invMass + massB.invMass);

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

static const float percent = 0.8f; // Penetration percentage to correct
static const float slop = 0.01f; // Penetration allowance
void PositionalCorrection(const Collision& manifold)
{
    const RigidBodyComponent* rigidbodyA = manifold.entityA->GetRigidBodyComponent();
    const RigidBodyComponent* rigidbodyB = manifold.entityB->GetRigidBodyComponent();
    float invMassA = rigidbodyA->GetMassData().invMass;
    float invMassB = rigidbodyB->GetMassData().invMass;
    const Vector2& correction = (std::max(manifold.penetration - slop, 0.0f) / (invMassA + invMassB)) * manifold.normal * percent;
    if (!rigidbodyA->IsStatic())
    {
        manifold.entityA->position -= correction * invMassA;
    }
    if (!rigidbodyB->IsStatic())
    {
        manifold.entityB->position += correction * invMassB;
    }
}

void CollisionManager::CheckCollisions(float deltaTime)
{
    for (Entity* entity : m_sandbox->GetEntities())
    {
        // Do not check for static object
        RigidBodyComponent* rigidbody = entity->GetRigidBodyComponent();
        if (rigidbody == nullptr || rigidbody->IsStatic())
        {
            continue;
        }

        for (Entity* other : m_quadTree->Search(entity))
        {
            // Do not check for self-collision
            if (entity == other)
            {
                continue;
            }

            const Collision& manifold = Solve(entity, other);
            if (manifold.penetration != 0)
            {
                ResolveCollision(manifold);
                PositionalCorrection(manifold);
            }
        }
    }
}

void CollisionManager::SetRootSize(int width, int height)
{
    delete m_quadTree;
	m_quadTree = new QuadTree<Entity>(AABB(Vector2(), Vector2(FLOAT(width), FLOAT(height))));
}

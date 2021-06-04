#include <algorithm>

#include "Precompiled.h"

CollisionManager::CollisionManager(const Sandbox* sandbox) :
	m_sandbox(sandbox),
	m_quadTree(nullptr)
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

void CollisionManager::Update(float delta)
{
	UpdateQuadTree();
	CheckCollisions(delta);
}

void CollisionManager::UpdateQuadTree()
{
	m_quadTree->Clear();
	for (Entity* object : m_sandbox->GetEntities())
	{
		m_quadTree->Insert(object);
	}
}
/*
void CollisionManager::ResolveCollision(const Entity* entityA, const Entity* entityB)
{
    // Calculate relative velocity
    const Vector2& rv = *entityB->velocity - *entityA->velocity;

    // TODO 
    Vector2 normal;
    // Calculate relative velocity in terms of the normal direction
    float velAlongNormal = rv.Dot(normal);

    // Do not resolve if velocities are separating
    if (velAlongNormal > 0)
    {
        return;
    }

    // Calculate restitution
    float e = MIN(entityA->GetRestitution(), entityB->GetRestitution());

    // Calculate impulse scalar
    float entityAMass = entityA->GetRigidBodyComponent()->GetMass();
    float entityBMass = entityB->GetRigidBodyComponent()->GetMass();
    float entityAInvMass = entityA->GetRigidBodyComponent()->GetInvMass();
    float entityBInvMass = entityB->GetRigidBodyComponent()->GetInvMass();

    float j = -(1 + e) * velAlongNormal / (entityAInvMass + entityBInvMass);

    // Apply impule
    const Vector2& impulse = normal * j;

    float massSum = entityAMass + entityBMass;
    float ratioA = entityAMass / massSum;
    *entityA->velocity -= impulse * ratioA;
    float ratioB = entityBMass / massSum;
    *entityB->velocity += impulse * ratioB;
}

void PositionalCorrection(const Entity* entityA, const Entity* entityB)
{
    static const float percent = 0.4f; // Penetration percentage to correct
    static const float slop = 0.05f; // Penetration allowance
    float invMassA = entityA->GetRigidBodyComponent()->GetInvMass();
    float invMassB = entityB->GetRigidBodyComponent()->GetInvMass();
    const Vector2& correction = (std::max(penetration - slop, 0.0f) / (invMassA + invMassB)) * normal * percent;
    *entityA->position -= correction * invMassA;
    *entityB->position += correction * invMassB;
}
*/

void CollisionManager::CheckCollisions(float delta)
{
    for (Entity* entity : m_sandbox->GetEntities())
    {
        // Do not check for static object
        RigidBodyComponent* rigidbody = entity->GetRigidBodyComponent();
        if (rigidbody == nullptr || rigidbody->GetType() == RigidbodyType::STATIC)
        {
            continue;
        }

        std::vector<Entity*> search = m_quadTree->Search(entity);

        // Sort entities from the largest y coordinate to the lowest one
        std::sort(search.begin(), search.end(),
            [](const Entity* a, const Entity* b) { return a->position.y < b->position.y; });

        for (Entity* other : search)
        {
            // Do not check for self-collision
            if (entity == other)
            {
                continue;
            }

            RigidBodyComponent* otherRigidbody = other->GetRigidBodyComponent();
            if (otherRigidbody == nullptr)
            {
                continue;
            }

            const AABB& md = other->GetAABB().MinkowskiDifference(entity->GetAABB());

            // Check for discrete AABB collision and then check for full collision
            if (md.min.x <= 0 && md.max.x >= 0
                && md.min.y <= 0 && md.max.y >= 0
                && entity->Intersects(other))
            {

                Vector2 penetrationVector = entity->ComputePenetrationVector(other);
                if (penetrationVector == Vector2::ZERO)
                {
                    penetrationVector = md.ClosestPointOnBoundsToPoint(Vector2::ZERO);
                }
                entity->position += penetrationVector;

                // See: https://en.wikipedia.org/wiki/Elastic_collision#two-dimensional
                // Section: Two-dimensional collision with two moving objects
                const Vector2& x1 = entity->position;
                const Vector2& x2 = other->position;
                const Vector2& v1 = entity->velocity;
                const Vector2& v2 = other->velocity;
                if(otherRigidbody->GetType() == RigidbodyType::STATIC)
                {
                    entity->velocity -= (x1 - x2) * 2.0f * (v1 - v2).Dot(x1 - x2) / (x1 - x2).LengthSq();
                }
                else
                {
                    float m1 = rigidbody->GetMass();
                    float m2 = otherRigidbody->GetMass();
                    entity->velocity -= (x1 - x2) * (2.0f * m2) / (m1 + m2) * (v1 - v2).Dot(x1 - x2) / (x1 - x2).LengthSq();
                    other->velocity -= (x2 - x1) * (2.0f * m1) / (m1 + m2) * (v2 - v1).Dot(x2 - x1) / (x2 - x1).LengthSq();
                }
            }
            // Swept AABB collision detection
            else {
                Vector2 relativeMotion = (entity->velocity - other->velocity) * delta;

                float intersectFraction = md.ComputeRayIntersectionFraction(Vector2::ZERO, relativeMotion);
                if (intersectFraction < FLOAT_INFINITY)
                {
                    entity->position += entity->velocity * (delta * intersectFraction);
                    if (otherRigidbody->GetType() == RigidbodyType::DYNAMIC)
                    {
                        other->position += other->velocity * (delta * intersectFraction);
                    }

                    // TODO: Two-dimensional collision with two moving objects
                    relativeMotion.Normalize();
                    const Vector2& tangent = relativeMotion.Tangent();
                    entity->velocity = tangent * entity->velocity.Dot(tangent);
                    if (otherRigidbody->GetType() == RigidbodyType::DYNAMIC)
                    {
                        other->velocity = tangent * other->velocity.Dot(tangent);
                    }
                }
            }
        }
    }
}

void CollisionManager::SetRootSize(int width, int height)
{
    delete m_quadTree;
	m_quadTree = new QuadTree<Entity>(AABB(Vector2(), Vector2(width, height)));
}

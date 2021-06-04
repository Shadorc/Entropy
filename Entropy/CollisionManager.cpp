#include "Sandbox.h"
#include "Entity.h"
#include "CollisionManager.h"
#include "RigidBodyComponent.h"
#include "Config.h"
#include <algorithm>

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
	for (const Entity* object : m_sandbox->GetEntities())
	{
		m_quadTree->Insert(object);
	}
}

void CollisionManager::CheckCollisions(float delta)
{
    for (const Entity* entity : m_sandbox->GetEntities())
    {
        // Do not check for static object
        RigidBodyComponent* rigidbody = entity->GetRigidBodyComponent();
        if (rigidbody == nullptr || rigidbody->GetType() == Type::STATIC)
        {
            continue;
        }

        std::vector<const Entity*> search = m_quadTree->Search(entity);

        // Sort entities from the largest y coordinate to the lowest one
        std::sort(search.begin(), search.end(),
            [](const Entity* a, const Entity* b) { return a->GetPosition()->y < b->GetPosition()->y; });

        for (const Entity* other : search)
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
                *entity->GetPosition() += penetrationVector;

                // See: https://en.wikipedia.org/wiki/Elastic_collision#two-dimensional
                // Section: Two-dimensional collision with two moving objects
                const Vector2& x1 = *entity->GetPosition();
                const Vector2& x2 = *other->GetPosition();
                const Vector2& v1 = *entity->GetVelocity();
                const Vector2& v2 = *other->GetVelocity();
                if(otherRigidbody->GetType() == Type::STATIC)
                {
                    *entity->GetVelocity() -= (x1 - x2) * 2.0f * (v1 - v2).Dot(x1 - x2) / (x1 - x2).LengthSq();
                }
                else
                {
                    float m1 = rigidbody->GetMass();
                    float m2 = otherRigidbody->GetMass();
                    *entity->GetVelocity() -= (x1 - x2) * (2.0f * m2) / (m1 + m2) * (v1 - v2).Dot(x1 - x2) / (x1 - x2).LengthSq();
                    *other->GetVelocity() -= (x2 - x1) * (2.0f * m1) / (m1 + m2) * (v2 - v1).Dot(x2 - x1) / (x2 - x1).LengthSq();
                }
            }
            // Swept AABB collision detection
            else {
                Vector2 relativeMotion = (*entity->GetVelocity() - *other->GetVelocity()) * delta;

                float intersectFraction = md.ComputeRayIntersectionFraction(Vector2::ZERO, relativeMotion);
                if (intersectFraction < FLOAT_INFINITY)
                {
                    *entity->GetPosition() += *entity->GetVelocity() * (delta * intersectFraction);
                    if (otherRigidbody->GetType() == Type::DYNAMIC)
                    {
                        *other->GetPosition() += *other->GetVelocity() * (delta * intersectFraction);
                    }

                    // TODO: Two-dimensional collision with two moving objects
                    relativeMotion.Normalize();
                    const Vector2& tangent = relativeMotion.Tangent();
                    *entity->GetVelocity() = tangent * entity->GetVelocity()->Dot(tangent);
                    if (otherRigidbody->GetType() == Type::DYNAMIC)
                    {
                        *other->GetVelocity() = tangent * other->GetVelocity()->Dot(tangent);
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

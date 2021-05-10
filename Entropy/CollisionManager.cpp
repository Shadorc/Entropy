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
    for (const Entity* entity : m_sandbox->GetEntities()) {
        // Do not check for static object
        RigidBodyComponent* rigidBodyComponent = entity->GetComponent<RigidBodyComponent>();
        if (rigidBodyComponent == nullptr) {
            continue;
        }

        std::vector<const Entity*> search = m_quadTree->Search(entity);

        // Sort entities from the largest y coordinate to the lowest one
        std::sort(search.begin(), search.end(),
            [](const Entity* a, const Entity* b) { return a->GetPosition()->m_y < b->GetPosition()->m_y; });

        for (const Entity* other : search) {
            // Do not check for self-collision
            if (entity == other) {
                continue;
            }

            AABB md = other->GetAABB().MinkowskiDifference(entity->GetAABB());

            // Check for discrete AABB collision and then check for full collision
            if (md.GetTopLeft()->m_x <= 0 && md.GetBottomRight()->m_x >= 0
                && md.GetTopLeft()->m_y <= 0 && md.GetBottomRight()->m_y >= 0
                && entity->Intersects(other)) {

                Vector2 penetrationVector = entity->ComputePenetrationVector(other);
                if (penetrationVector == Vector2::ZERO) {
                    penetrationVector = md.ClosestPointOnBoundsToPoint(Vector2::ZERO);
                }
                *entity->GetPosition() += penetrationVector;

                penetrationVector.Normalize();
                Vector2 tangent = penetrationVector.Tangent();
                *entity->GetVelocity() = tangent * entity->GetVelocity()->Dot(tangent);
                *other->GetVelocity() = tangent * other->GetVelocity()->Dot(tangent);
            }
            // Swept AABB collision detection
            else {
                Vector2 relativeMotion = (*entity->GetVelocity() - *other->GetVelocity()) * delta;

                float intersectFraction = md.ComputeRayIntersectionFraction(Vector2::ZERO, relativeMotion);
                if (intersectFraction < FLOAT_INFINITY)
                {
                    *entity->GetPosition() += *entity->GetVelocity() * (delta * intersectFraction);
                    *other->GetPosition() += *other->GetVelocity() * (delta * intersectFraction);

                    relativeMotion.Normalize();
                    Vector2 tangent = relativeMotion.Tangent();
                    *entity->GetVelocity() = tangent * entity->GetVelocity()->Dot(tangent);
                    *other->GetVelocity() = tangent * other->GetVelocity()->Dot(tangent);
                }
            }
        }
    }
}

void CollisionManager::SetRootSize(int width, int height)
{
	if (m_quadTree != nullptr)
	{
		m_quadTree->Clear();
		delete m_quadTree;
	}
	
	m_quadTree = new QuadTree<Entity>(new AABB(Vector2(), Vector2(width, height)));
}

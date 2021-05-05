#include "Sandbox.h"
#include "CollisionManager.h"
#include <algorithm>
#include <limits>

CollisionManager::CollisionManager(const Sandbox* sandbox) :
	m_sandbox(sandbox),
	m_quadTree(nullptr)
{

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
        RigidBodyComponent* rigidBodyComponent = entity->GetRigidBodyComponent();
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

            /* TODO
            // Check for discrete AABB collision and then check for full collision
            if (md.GetTopLeft()->m_x <= 0 && md.GetBottomRight()->m_x >= 0
                && md.GetTopLeft()->m_y <= 0 && md.GetBottomRight()->m_y >= 0
                && entity.getHitboxShape().intersects(other.getHitboxShape())) {

                Vector2 penetrationVector = entity->GetHitboxShape().penetrationVector(other.getHitboxShape());
                if (penetrationVector == Vector2::ZERO) {
                    penetrationVector = md.closestPointOnBoundsToPoint(Vector.ZERO);
                }
                *entity->GetPosition() += penetrationVector;

                Vector2 tangent = penetrationVector.Normalize().Tangent();
                *entity->GetVelocity() = tangent * entity->GetVelocity()->Dot(tangent);
                *other->GetVelocity() = tangent * other->GetVelocity()->Dot(tangent);
            }
            // Swept AABB collision detection
            else {
                Vector2 relativeMotion = (*entity->GetVelocity() - *other->GetVelocity()) * delta;

                float intersectFraction = md.getRayIntersectionFraction(Vector2::ZERO, relativeMotion);
                if (intersectFraction < std::numeric_limits<float>::max())
                {
                    *entity->GetPosition() += *entity->GetVelocity() * (delta * intersectFraction);
                    *other->GetPosition() += *other->GetVelocity() * (delta * intersectFraction);

                    Vector2 tangent = relativeMotion.Normalize().Tangent();
                    *entity->GetVelocity() = tangent * entity->GetVelocity()->Dot(tangent);
                    *other->GetVelocity() = tangent * other->GetVelocity()->Dot(tangent);
                }
            }
            */
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

#include "Precompiled.h"

static const Vector2 GRAVITY = Vector2(0.0f, 9.8f);

GravityComponent::GravityComponent(Entity* entity)
	: Component(entity)
{
	m_entity->GetRigidBodyComponent()->AddForce(const_cast<Vector2&>(GRAVITY));
}

void GravityComponent::Update(float deltaTime)
{

}

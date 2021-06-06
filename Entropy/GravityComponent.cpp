#include "Precompiled.h"

const Vector2 GravityComponent::GRAVITY(0, 9.8f);

GravityComponent::GravityComponent(Entity* entity)
	: Component(entity)
{
	m_entity->GetRigidBodyComponent()->AddForce(const_cast<Vector2&>(GRAVITY));
}

void GravityComponent::Update(float deltaTime)
{

}

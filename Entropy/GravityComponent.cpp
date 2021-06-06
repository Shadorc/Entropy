#include "Precompiled.h"

static const Vector2 GRAVITY = Vector2(0.0f, 50.0f);

GravityComponent::GravityComponent(Entity* entity)
	: Component(entity)
{

}

void GravityComponent::Update(float deltaTime)
{
	m_entity->GetRigidbodyComponent()->AddForce(GRAVITY);
}

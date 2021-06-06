#include "Precompiled.h"

static const Vector2 GRAVITY = Vector2(0.0f, 50.0f);

GravityComponent::GravityComponent(Entity* entity)
	: Component(entity)
{

}

void GravityComponent::Update(float deltaTime)
{
	RigidbodyComponent* rigidbody = m_entity->GetRigidbodyComponent();
	rigidbody->AddForce(rigidbody->GetMassData().mass * GRAVITY);
}

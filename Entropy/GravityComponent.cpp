#include "Precompiled.h"

const Vector2 GravityComponent::GRAVITY = Vector2(0.0f, 9.8f * 10.0f);

GravityComponent::GravityComponent(Entity* entity)
	: Component(entity)
{

}

void GravityComponent::Update(float deltaTime)
{
	RigidbodyComponent* rigidbody = m_Entity->GetRigidbodyComponent();
	rigidbody->AddForce(rigidbody->GetMassData().mass * GRAVITY);
}

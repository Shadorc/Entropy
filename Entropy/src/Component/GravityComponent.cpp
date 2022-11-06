#include "Precompiled.h"

const Vector2 GravityComponent::GRAVITY = Vector2(0.0f, 9.8f * 20.0f);

GravityComponent::GravityComponent(const std::weak_ptr<Entity>& entity)
	: Component(entity)
{

}

void GravityComponent::Update(float deltaTime)
{
	const auto& entity = m_Entity.lock();
	if (entity)
	{
		RigidbodyComponent* rigidbody = entity->GetComponent<RigidbodyComponent>();
		rigidbody->AddForce(rigidbody->GetMassData().mass * GRAVITY);
	}
}

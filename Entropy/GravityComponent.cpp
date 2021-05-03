#include "GravityComponent.h"
#include "RigidBodyComponent.h"

#define GRAVITY 91.4f

GravityComponent::GravityComponent(const Entity* entity): 
	Component(entity)
{
	m_entity->GetRigidBodyComponent()->AddForce(new Vector2(0, GRAVITY));
}

void GravityComponent::Update(float delta)
{

}

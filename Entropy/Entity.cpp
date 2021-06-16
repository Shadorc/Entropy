#include "Precompiled.h"

static int s_id = 0;

Entity::Entity(float x, float y) 
	: m_Id(s_id++)
	, m_Position(x, y)
	, velocity()
	, force()
	, m_Orientation(0.0f)
	, angularVelocity(0.0f)
	, torque(0.0f)
	, m_RigidbodyComponentCache(nullptr)
	, m_AabbCache(nullptr)
{

}

Entity::~Entity()
{
	ENTROPY_DELETE(m_RigidbodyComponentCache);
	ENTROPY_DELETE(m_AabbCache);
	for (Component* component : m_Components)
	{
		ENTROPY_DELETE(component);
	}
	m_Components.clear();
}

// Cache Rigidbody component
RigidbodyComponent* Entity::GetRigidbodyComponent() const
{
	if (m_RigidbodyComponentCache == nullptr)
	{
		m_RigidbodyComponentCache = GetComponent<RigidbodyComponent>();
	}
	return m_RigidbodyComponentCache;
}

void Entity::AddComponent(Component* component)
{
	m_Components.push_back(component);
}

void Entity::ApplyImpulse(const Vector2& impulse, const Vector2& contactVector)
{
	const RigidbodyComponent* rigidbody = GetRigidbodyComponent();
	ENTROPY_ASSERT(rigidbody, "Rigidbody absent");
	velocity += rigidbody->GetMassData().invMass * impulse;
	angularVelocity += rigidbody->GetMassData().invInertia * contactVector.Cross(impulse);
}

void Entity::Translate(const Vector2& vector)
{
	ENTROPY_DELETE(m_AabbCache);
	m_Position += vector;
}

uint Entity::GetId() const
{
	return m_Id;
}

const Vector2 Entity::GetPosition() const
{
	return m_Position;
}

const AABB* Entity::GetAABB() const
{
	if (m_AabbCache == nullptr)
	{
		m_AabbCache = ComputeAABB();
	}
	return m_AabbCache;
}

void Entity::Update(float deltaTime)
{
	for (Component* component : m_Components)
	{
		component->Update(deltaTime);
	}
}

bool Entity::operator==(Entity& other) const
{
	return m_Id == other.m_Id;
}

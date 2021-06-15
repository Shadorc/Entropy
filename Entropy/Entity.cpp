#include "Precompiled.h"

static int s_id = 0;

Entity::Entity(float x, float y) 
	: m_id(s_id++)
	, m_position(x, y)
	, velocity()
	, force()
	, m_orientation(0.0f)
	, angularVelocity(0.0f)
	, torque(0.0f)
	, m_rigidbodyComponentCache(nullptr)
	, m_aabbCache(nullptr)
{

}

Entity::~Entity()
{
	ENTROPY_DELETE(m_rigidbodyComponentCache);
	for (Component* component : m_components)
	{
		ENTROPY_DELETE(component);
	}
	m_components.clear();
}

// Cache Rigidbody component
RigidbodyComponent* Entity::GetRigidbodyComponent() const
{
	if (m_rigidbodyComponentCache == nullptr)
	{
		m_rigidbodyComponentCache = GetComponent<RigidbodyComponent>();
	}
	return m_rigidbodyComponentCache;
}

void Entity::AddComponent(Component* component)
{
	m_components.push_back(component);
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
	ENTROPY_DELETE(m_aabbCache);
	m_position += vector;
}

unsigned int Entity::GetId() const
{
	return m_id;
}

const Vector2 Entity::GetPosition() const
{
	return m_position;
}

const AABB* Entity::GetAABB() const
{
	if (m_aabbCache == nullptr)
	{
		m_aabbCache = ComputeAABB();
	}
	return m_aabbCache;
}

void Entity::Update(float deltaTime)
{
	for (Component* component : m_components)
	{
		component->Update(deltaTime);
	}
}

bool Entity::operator==(Entity& other) const
{
	return m_id == other.m_id;
}

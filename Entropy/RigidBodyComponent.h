#pragma once

#include <vector>

#include "Component.h"
#include "Vector2.h"

struct Force 
{
	Vector2 vector;
	bool isInstant;

	Force(Vector2& vector, bool isInstant)
		: vector(vector)
		, isInstant(isInstant)
	{

	}
};

enum class RigidbodyType
{
	STATIC, 
	DYNAMIC,
	COUNT
};

class Entity;
class RigidBodyComponent: public Component
{
private:
	const RigidbodyType m_type;
	const float m_mass;
	// Pre-computed value of 1 / m_mass
	const float m_invMass;
	std::vector<Force> m_forces;
	Vector2 m_acceleration;

public:
	RigidBodyComponent(Entity* entity, RigidbodyType type, float mass);
	~RigidBodyComponent();

	void AddForce(Vector2& force);
	void AddInstantForce(Vector2& force);
	void Update(float deltaTime) override;

	RigidbodyType GetType() const;
	float GetMass() const;
	float GetInvMass() const;
};


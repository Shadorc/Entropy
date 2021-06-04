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

enum class ComponentType
{
	STATIC, 
	DYNAMIC,
	COUNT
};

class Entity;
class RigidBodyComponent: public Component
{
private:
	const ComponentType m_type;
	const float m_mass;
	// Pre-computed value of 1 / m_mass
	const float m_invMass;
	std::vector<Force> m_forces;
	Vector2* m_acceleration;

public:
	RigidBodyComponent(const Entity* entity, const ComponentType type, const float mass);
	~RigidBodyComponent();

	void AddForce(Vector2& force);
	void AddInstantForce(Vector2& force);
	void Update(float delta) override;

	ComponentType GetType() const;
	float GetMass() const;
	float GetInvMass() const;
};


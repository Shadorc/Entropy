#pragma once

#include "Component.h"
#include "Vector2.h"
#include <vector>

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

enum class Type
{
	STATIC, 
	DYNAMIC
};

class Entity;
class RigidBodyComponent: public Component
{
private:
	Type m_type;
	float m_mass;
	std::vector<Force> m_forces;
	Vector2* m_acceleration;

public:
	RigidBodyComponent(const Entity* entity, Type type, float mass);
	~RigidBodyComponent();

	void AddForce(Vector2& force);
	void AddInstantForce(Vector2& force);
	void Update(float delta) override;

	Type GetType() const;
	float GetMass() const;
};


#pragma once

#include "Math/Vector2.h"
#include "Entity/Entity.h"

struct Collision
{
	Entity* entityA;
	Entity* entityB;

	float penetration;
	Vector2 normal;
	std::vector<Vector2> contacts;

	Collision(Entity* entityA, Entity* entityB);
};


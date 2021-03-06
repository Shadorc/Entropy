#pragma once

#include "Vector2.h"

class Entity;
struct Collision
{
	Entity* entityA;
	Entity* entityB;

	float penetration;
	Vector2 normal;
	std::vector<Vector2> contacts;

	Collision(Entity* entityA, Entity* entityB);
};


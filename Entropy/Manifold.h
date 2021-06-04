#pragma once

#include "Vector2.h"

class Entity;
struct Manifold
{
	const Entity* entityA;
	const Entity* entityB;
	float penetration;
	Vector2 normal;

	Manifold(const Entity* entityA, const Entity* entityB);
};


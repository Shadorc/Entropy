#include "Precompiled.h"

Collision::Collision(Entity* entityA, Entity* entityB)
	: entityA(entityA)
	, entityB(entityB)
	, penetration(0)
	, normal(Vector2::ZERO)
	, contacts()
{
}

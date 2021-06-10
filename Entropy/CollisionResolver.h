#pragma once
#include "Entity.h"

struct Collision;

typedef void (*CollisionCallback)(Collision& collision);

extern CollisionCallback Dispatch[(int) EntityType::COUNT][(int) EntityType::COUNT];

Collision Solve(Entity* entityA, Entity* entityB);
void CircleToCircle(Collision& collision);
void CircleToRectangle(Collision& collision);
void RectangleToCircle(Collision& collision);
void RectangleToRectangle(Collision& collision);
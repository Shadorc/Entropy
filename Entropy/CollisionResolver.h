#pragma once
#include "Entity.h"

struct Manifold;

typedef void (*CollisionCallback)(Manifold* manifold);

extern CollisionCallback Dispatch[static_cast<int>(EntityType::Count)][static_cast<int>(EntityType::Count)];

void CircleToCircle(Manifold* manifold);
void CircleToRectangle(Manifold* manifold);
void RectangleToCircle(Manifold* manifold);
void RectangleToRectangle(Manifold* manifold);
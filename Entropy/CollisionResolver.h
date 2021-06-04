#pragma once
#include "Entity.h"
#include "Manifold.h"

typedef void (*CollisionCallback)(Manifold* manifold);

extern CollisionCallback Dispatch[static_cast<int>(Type::Count)][static_cast<int>(Type::Count)];

void CircleToCircle(Manifold* manifold);
void CircleToRectangle(Manifold* manifold);
void RectangleToCircle(Manifold* manifold);
void RectangleToRectangle(Manifold* manifold);
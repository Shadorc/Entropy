#pragma once

constexpr int WIDTH = 720;
constexpr int HEIGHT = 720;

constexpr float DELTA_TIME = 1.0f / 60.0f;		// Target 60 FPS

constexpr float PENETRATION_PERCENT = 0.4f;		// Penetration percentage to correct
constexpr float PENETRATION_ALLOWANCE = 0.05f;	// Penetration allowance

constexpr int CIRCLE_VERTICES = 24;

#ifdef _DEBUG
#define ENTROPY_DEBUG
#endif

#ifdef ENTROPY_DEBUG
static size_t s_allocatedMemory = 0;

#define ENTROPY_NEW(x, ...) ([&]() {\
	s_allocatedMemory += sizeof(x);\
	return new x(__VA_ARGS__);\
})()

#define ENTROPY_DELETE(x) ([&]() {\
	s_allocatedMemory -= sizeof(x);\
	return delete x;\
})()
#endif // ENTROPY_DEBUG

#include <math.h>
#include <vector>
#include <GL/glut.h>
#include <algorithm>
#include <ostream>

#ifdef ENTROPY_DEBUG
#include "DebugMode.h"
#endif // ENTROPY_DEBUG

#include "Render.h"
#include "Vector2.h"
#include "Entity.h"
#include "Circle.h"
#include "Rectangle.h"
#include "AABB.h"
#include "AABBObject.h"
#include "CollisionManager.h"
#include "CollisionResolver.h"
#include "QuadTree.h"
#include "Collision.h"
#include "Component.h"
#include "GravityComponent.h"
#include "RigidbodyComponent.h"
#include "Sandbox.h"
#include "MassData.h"
#include "Material.h"
#include "Pair.h"
#include "MathIE.h"

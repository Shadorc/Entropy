#pragma once

typedef unsigned int uint;

constexpr uint WIDTH = 720;
constexpr uint HEIGHT = 720;

constexpr float DELTA_TIME = 1.0f / 60.0f;		// Target 60 FPS

constexpr float PENETRATION_PERCENT = 0.4f;		// Penetration percentage to correct
constexpr float PENETRATION_ALLOWANCE = 0.05f;	// Penetration allowance
constexpr uint COLLISIONS_SOLVER_ITERATIONS = 10;

constexpr uint CIRCLE_VERTICES = 24;

#ifdef _DEBUG
#define ENTROPY_DEBUG
#endif

#define ENTROPY_NEW(x, ...) \
	([&]() { \
		return new x(__VA_ARGS__); \
	})()

#define ENTROPY_DELETE(x) \
	if (x != nullptr) \
	{ \
		delete x; \
		x = nullptr; \
	}

#ifdef ENTROPY_DEBUG
#define ENTROPY_LOG(x) std::cout << x << std::endl
#else
#define ENTROPY_LOG(x)
#endif // ENTROPY_DEBUG

#ifdef ENTROPY_DEBUG
#define ENTROPY_ASSERT_WITH_REASON(condition, reason) \
	if (!(condition)) \
	{ \
		ENTROPY_LOG(reason); \
		assert(condition); \
	}
#else
#define ENTROPY_ASSERT_WITH_REASON(condition, reason)
#endif // ENTROPY_DEBUG

#ifdef ENTROPY_DEBUG
#define ENTROPY_ASSERT(condition) \
	if (!(condition)) \
	{ \
		assert(condition); \
	}
#else
#define ENTROPY_ASSERT(condition)
#endif // ENTROPY_DEBUG

#include <math.h>
#include <vector>
#include <GL/glut.h>
#include <algorithm>
#include <ostream>
#include <iostream>
#include <assert.h>
#include <string>

#ifdef ENTROPY_DEBUG
#include "DebugMode.h"
#endif // ENTROPY_DEBUG

#include "Raycast.h"
#include "Render.h"
#include "Vector2.h"
#include "Matrix22.h"
#include "Entity.h"
#include "Circle.h"
#include "Polygon.h"
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
#include "MaterialData.h"
#include "Pair.h"
#include "MathIE.h"

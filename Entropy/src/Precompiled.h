#pragma once

using uint = unsigned int;

constexpr uint WIDTH = 1280;
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
#include "Debug/DebugMode.h"
#endif // ENTROPY_DEBUG

#include "Math/Raycast.h"
#include "Util/Render.h"
#include "Math/Vector2.h"
#include "Math/Matrix22.h"
#include "Entity/Entity.h"
#include "Entity/Circle.h"
#include "Entity/Polygon.h"
#include "Quadtree/AABB.h"
#include "Quadtree/AABBObject.h"
#include "Physic/CollisionManager.h"
#include "Physic/CollisionResolver.h"
#include "Quadtree/QuadTree.h"
#include "Physic/Collision.h"
#include "Component/Component.h"
#include "Component/GravityComponent.h"
#include "Component/RigidbodyComponent.h"
#include "Sandbox.h"
#include "Physic/Data/MassData.h"
#include "Physic/Data/MaterialData.h"
#include "Util/Pair.h"
#include "Math/MathIE.h"

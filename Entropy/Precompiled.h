#pragma once

constexpr int WIDTH = 720;
constexpr int HEIGHT = 720;

#include <math.h>
#include <vector>
#include <GL/glut.h>

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
#include "RigidBodyComponent.h"
#include "Sandbox.h"
#include "MassData.h"
#include "Material.h"

#define FLOAT_INFINITY std::numeric_limits<float>::infinity()
#define RAND_INT(max) rand() % max + 1
#define INT(a) static_cast<int>(a)
#define UINT(a) static_cast<unsigned int>(a)
#define FLOAT(a) static_cast<float>(a)
#define CLAMP(value, min, max) (value < min ? min : (value > max ? max : value))
#define MIN(a, b) a < b ? a : b
#define MAX(a, b) a > b ? a : b
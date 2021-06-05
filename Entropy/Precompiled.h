#pragma once

#define WIDTH 720
#define HEIGHT 720

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
#include "MathUtil.h"
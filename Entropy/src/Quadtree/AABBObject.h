#pragma once

#include "Quadtree/AABB.h"

class AABBObject
{
public:
	virtual const AABB* GetAABB() const = 0;
};


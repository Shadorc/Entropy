#pragma once

#include "AABB.h"

class AABBObject
{
public:
	virtual AABB GetAABB() const = 0;
};


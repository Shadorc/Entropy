#pragma once

struct AABB;
class AABBObject
{
public:
	virtual const AABB* GetAABB() const = 0;
};


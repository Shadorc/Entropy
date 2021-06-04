#pragma once

struct AABB;
class AABBObject
{
public:
	virtual AABB GetAABB() const = 0;
};


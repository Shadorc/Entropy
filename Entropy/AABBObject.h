#pragma once

class AABB;
class AABBObject
{
public:
	virtual AABB GetAABB() const = 0;
};


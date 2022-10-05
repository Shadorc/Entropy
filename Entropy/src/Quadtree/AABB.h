#pragma once

#include "Math/Vector2.h"

struct AABB
{
	const float minX;
	const float minY;
	const float maxX;
	const float maxY;

	AABB(float minX, float minY, float maxX, float maxY);

	float GetX() const;
	float GetY() const;
	float GetWidth() const;
	float GetHeight() const;

	bool IntersectsWith(const AABB* other) const;
};


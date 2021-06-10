#pragma once

#include "Vector2.h"

struct AABB
{
	const Vector2 min;
	const Vector2 max;

	AABB(const Vector2& min, const Vector2& max);

	float GetX() const;
	float GetY() const;
	float GetWidth() const;
	float GetHeight() const;

	bool IntersectsWith(const AABB& other) const;
};


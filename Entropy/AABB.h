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
	AABB MinkowskiDifference(const AABB& other) const;
	Vector2 ClosestPointOnBoundsToPoint(const Vector2& point) const;
	float ComputeRayIntersectionFraction(const Vector2& origin, const Vector2& direction) const;
	float ComputeRayIntersectionFractionOfFirstRay(const Vector2& originA, const Vector2& endA, 
		const Vector2& originB, const Vector2& endB) const;
};


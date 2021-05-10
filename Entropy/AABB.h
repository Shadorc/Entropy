#pragma once

#include "Vector2.h"
#include <limits> 

#define FLOAT_INFINITY std::numeric_limits<float>::infinity()

class AABB
{
private:
	const Vector2* m_topLeft;
	const Vector2* m_bottomRight;

	float ComputeRayIntersectionFractionOfFirstRay(const Vector2& originA, const Vector2& endA, 
		const Vector2& originB, const Vector2& endB) const;

public:
	AABB(Vector2 topLeft, Vector2 bottomRight);
	AABB(Vector2* topLeft, Vector2* bottomRight);
	~AABB();

	float GetX() const;
	float GetY() const;
	float GetWidth() const;
	float GetHeight() const;
	const Vector2* GetTopLeft() const;
	const Vector2* GetBottomRight() const;

	bool IntersectsWith(const AABB& other) const;
	AABB MinkowskiDifference(const AABB& other) const;
	Vector2 ClosestPointOnBoundsToPoint(const Vector2& point) const;
	float ComputeRayIntersectionFraction(const Vector2& origin, const Vector2 direction) const;
};


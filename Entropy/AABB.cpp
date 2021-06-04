#include <limits>

#include "Precompiled.h"

AABB::AABB(const Vector2& topLeft, const Vector2& bottomRight) :
	min(topLeft),
	max(bottomRight)
{

}

float AABB::GetX() const
{
	return min.x;
}

float AABB::GetY() const
{
	return min.y;
}

float AABB::GetWidth() const
{
	return max.x - min.x;
}

float AABB::GetHeight() const
{
	return max.y - min.y;
}

bool AABB::IntersectsWith(const AABB& other) const
{
	// Exit with no intersection if found separated along an axis
	if (max.x < other.min.x || min.x > other.max.x)
	{
		return false;
	}
	if (max.y < other.min.y || min.y > other.max.y)
	{
		return false;
	}

	// No separating axis found, therefor there is at least one overlapping axis
	return true;
}

AABB AABB::MinkowskiDifference(const AABB& other) const
{
	const Vector2& topLeft = min - other.max;
	const Vector2& bottomRight = max - other.min;
	return AABB(topLeft, bottomRight);
}

Vector2 AABB::ClosestPointOnBoundsToPoint(const Vector2& point) const
{
	float minDist = abs(point.x - min.x);
	Vector2 boundsPoint(min.x, point.y);
	if (abs(max.x - point.x) < minDist)
	{
		minDist = abs(max.x - point.x);
		boundsPoint.x = max.x;
		boundsPoint.y = point.y;
	}
	if (abs(max.y - point.y) < minDist) 
	{
		minDist = abs(max.y - point.y);
		boundsPoint.x = point.x;
		boundsPoint.y = max.y;
	}
	if (abs(min.y - point.y) < minDist) 
	{
		minDist = abs(min.y - point.y);
		boundsPoint.x = point.x;
		boundsPoint.y = min.y;
	}
	return boundsPoint;
}

float AABB::ComputeRayIntersectionFraction(const Vector2& origin, const Vector2& direction) const
{
	const Vector2& end = origin + direction;
	float intersection1 = ComputeRayIntersectionFractionOfFirstRay(origin, end,
		Vector2(min.x, min.y), Vector2(min.x, max.y));
	float intersection2 = ComputeRayIntersectionFractionOfFirstRay(origin, end,
		Vector2(min.x, max.y), Vector2(max.x, max.y));
	float intersection3 = ComputeRayIntersectionFractionOfFirstRay(origin, end,
		Vector2(max.x, max.y), Vector2(max.x, min.y));
	float intersection4 = ComputeRayIntersectionFractionOfFirstRay(origin, end,
		Vector2(max.x, min.y), Vector2(min.x, min.y));
	return MIN(MIN(intersection1, intersection2), MIN(intersection3, intersection4));
}

float AABB::ComputeRayIntersectionFractionOfFirstRay(const Vector2& originA, const Vector2& endA, 
	const Vector2& originB, const Vector2& endB) const
{
	const Vector2& r = endA - originA;
	const Vector2& s = endB - originB;

	float numerator = (originB - originA).Cross(r);
	float denominator = r.Cross(s);

	if (numerator == 0 && denominator == 0) 
	{
		return FLOAT_INFINITY;
	}
	if (denominator == 0)
	{
		return FLOAT_INFINITY;
	}

	float u = numerator / denominator;
	float t = (originB - originA).Cross(s) / denominator;

	if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
	{
		return t;
	}
	return FLOAT_INFINITY;
}

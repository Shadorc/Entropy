#include "Vector2.h"
#include "AABB.h"
#include <math.h>
#include <limits>
#include "MathUtil.h"

AABB::AABB(Vector2 topLeft, Vector2 bottomRight):
	AABB(new Vector2(topLeft), new Vector2(bottomRight))
{
}

AABB::AABB(Vector2* topLeft, Vector2* bottomRight) :
	m_topLeft(topLeft),
	m_bottomRight(bottomRight)
{

}

AABB::~AABB() 
{
	delete m_topLeft;
	delete m_bottomRight;
}

float AABB::GetX() const
{
	return m_topLeft->x;
}

float AABB::GetY() const
{
	return m_topLeft->y;
}

float AABB::GetWidth() const
{
	return m_bottomRight->x - m_topLeft->x;
}

float AABB::GetHeight() const
{
	return m_bottomRight->y - m_topLeft->y;
}

const Vector2* AABB::GetTopLeft() const
{
	return m_topLeft;
}

const Vector2* AABB::GetBottomRight() const
{
	return m_bottomRight;
}

bool AABB::IntersectsWith(const AABB& other) const
{
	const AABB& md = other.MinkowskiDifference(*this);
	return md.m_topLeft->x <= 0 && md.m_bottomRight->x >= 0
		&& md.m_topLeft->y <= 0 && md.m_bottomRight->y >= 0;
}

AABB AABB::MinkowskiDifference(const AABB& other) const
{
	Vector2 topLeft = *m_topLeft - *other.m_bottomRight;
	Vector2 bottomRight = *m_bottomRight - *other.m_topLeft;
	return AABB(topLeft, bottomRight);
}

Vector2 AABB::ClosestPointOnBoundsToPoint(const Vector2& point) const
{
	float minDist = abs(point.x - m_topLeft->x);
	Vector2 boundsPoint(m_topLeft->x, point.y);
	if (abs(m_bottomRight->x - point.x) < minDist)
	{
		minDist = abs(m_bottomRight->x - point.x);
		boundsPoint.x = m_bottomRight->x;
		boundsPoint.y = point.y;
	}
	if (abs(m_bottomRight->y - point.y) < minDist) 
	{
		minDist = abs(m_bottomRight->y - point.y);
		boundsPoint.x = point.x;
		boundsPoint.y = m_bottomRight->y;
	}
	if (abs(m_topLeft->y - point.y) < minDist) 
	{
		minDist = abs(m_topLeft->y - point.y);
		boundsPoint.x = point.x;
		boundsPoint.y = m_topLeft->y;
	}
	return boundsPoint;
}

float AABB::ComputeRayIntersectionFraction(const Vector2& origin, const Vector2& direction) const
{
	Vector2 end = origin + direction;
	float intersection1 = ComputeRayIntersectionFractionOfFirstRay(origin, end,
		Vector2(m_topLeft->x, m_topLeft->y), Vector2(m_topLeft->x, m_bottomRight->y));
	float intersection2 = ComputeRayIntersectionFractionOfFirstRay(origin, end,
		Vector2(m_topLeft->x, m_bottomRight->y), Vector2(m_bottomRight->x, m_bottomRight->y));
	float intersection3 = ComputeRayIntersectionFractionOfFirstRay(origin, end,
		Vector2(m_bottomRight->x, m_bottomRight->y), Vector2(m_bottomRight->x, m_topLeft->y));
	float intersection4 = ComputeRayIntersectionFractionOfFirstRay(origin, end,
		Vector2(m_bottomRight->x, m_topLeft->y), Vector2(m_topLeft->x, m_topLeft->y));
	return MIN(MIN(intersection1, intersection2), MIN(intersection3, intersection4));
}

float AABB::ComputeRayIntersectionFractionOfFirstRay(const Vector2& originA, const Vector2& endA, 
	const Vector2& originB, const Vector2& endB) const
{
	Vector2 r = endA - originA;
	Vector2 s = endB - originB;

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

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
	return m_topLeft->m_x;
}

float AABB::GetY() const
{
	return m_topLeft->m_y;
}

float AABB::GetWidth() const
{
	return m_bottomRight->m_x - m_topLeft->m_x;
}

float AABB::GetHeight() const
{
	return m_bottomRight->m_y - m_topLeft->m_y;
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
	AABB md = other.MinkowskiDifference(*this);
	return md.m_topLeft->m_x <= 0 && md.m_bottomRight->m_x >= 0
		&& md.m_topLeft->m_y <= 0 && md.m_bottomRight->m_y >= 0;
}

AABB AABB::MinkowskiDifference(const AABB& other) const
{
	Vector2 topLeft = *m_topLeft - *other.m_bottomRight;
	Vector2 bottomRight = *m_bottomRight - *other.m_topLeft;
	return AABB(topLeft, bottomRight);
}

Vector2 AABB::ClosestPointOnBoundsToPoint(const Vector2& point) const
{
	float minDist = abs(point.m_x - m_topLeft->m_x);
	Vector2 boundsPoint(m_topLeft->m_x, point.m_y);
	if (abs(m_bottomRight->m_x - point.m_x) < minDist)
	{
		minDist = abs(m_bottomRight->m_x - point.m_x);
		boundsPoint = Vector2(m_bottomRight->m_x, point.m_y);
	}
	if (abs(m_bottomRight->m_y - point.m_y) < minDist) 
	{
		minDist = abs(m_bottomRight->m_y - point.m_y);
		boundsPoint = Vector2(point.m_x, m_bottomRight->m_y);
	}
	if (abs(m_topLeft->m_y - point.m_y) < minDist) 
	{
		minDist = abs(m_topLeft->m_y - point.m_y);
		boundsPoint = Vector2(point.m_x, m_topLeft->m_y);
	}
	return boundsPoint;
}

#define MIN(a, b) a < b ? a : b
float AABB::ComputeRayIntersectionFraction(const Vector2& origin, const Vector2& direction) const
{
	Vector2 end = origin + direction;
	float intersection1 = ComputeRayIntersectionFractionOfFirstRay(origin, end,
		Vector2(m_topLeft->m_x, m_topLeft->m_y), Vector2(m_topLeft->m_x, m_bottomRight->m_y));
	float intersection2 = ComputeRayIntersectionFractionOfFirstRay(origin, end,
		Vector2(m_topLeft->m_x, m_bottomRight->m_y), Vector2(m_bottomRight->m_x, m_bottomRight->m_y));
	float intersection3 = ComputeRayIntersectionFractionOfFirstRay(origin, end,
		Vector2(m_bottomRight->m_x, m_bottomRight->m_y), Vector2(m_bottomRight->m_x, m_topLeft->m_y));
	float intersection4 = ComputeRayIntersectionFractionOfFirstRay(origin, end,
		Vector2(m_bottomRight->m_x, m_topLeft->m_y), Vector2(m_topLeft->m_x, m_topLeft->m_y));
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

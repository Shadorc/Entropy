#include "Precompiled.h"

AABB::AABB(const Vector2& min, const Vector2& max) 
	: min(min)
	, max(max)
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

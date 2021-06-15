#include "Precompiled.h"

AABB::AABB(float minX, float minY, float maxX, float maxY)
	: minX(minX)
	, minY(minY)
	, maxX(maxX)
	, maxY(maxY)
{

}

float AABB::GetX() const
{
	return minX;
}

float AABB::GetY() const
{
	return minY;
}

float AABB::GetWidth() const
{
	return maxX - minX;
}

float AABB::GetHeight() const
{
	return maxY - minY;
}

bool AABB::IntersectsWith(const AABB* other) const
{
	// Exit with no intersection if found separated along an axis
	if (maxX < other->minX || minX > other->maxX)
	{
		return false;
	}
	if (maxY < other->minY || minY > other->maxY)
	{
		return false;
	}

	// No separating axis found, therefor there is at least one overlapping axis
	return true;
}

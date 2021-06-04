#include "CollisionResolver.h"

#include "Circle.h"
#include "Rectangle.h"
#include <math.h>
#include "MathUtil.h"

void CircleToCircle(Manifold* manifold)
{
	const entity::Circle* circleA = reinterpret_cast<const entity::Circle*>(manifold->entityA);
	const entity::Circle* circleB = reinterpret_cast<const entity::Circle*>(manifold->entityB);

	const Vector2& deltaPos = *circleB->GetPosition() - *circleA->GetPosition();
	float normalLenSq = deltaPos.LengthSq();
	float sumRadius = circleA->GetRadius() + circleB->GetRadius();

	if (normalLenSq >= sumRadius * sumRadius)
	{
		return;
	}

	if (normalLenSq == 0.0f)
	{
		manifold->penetration = circleA->GetRadius();
		manifold->normal = Vector2(1, 0);
	}
	else
	{
		float deltaPosLen = sqrt(normalLenSq);
		manifold->penetration = sumRadius - deltaPosLen;
		manifold->normal = deltaPos / deltaPosLen;
	}
}

void CircleToRectangle(Manifold* manifold)
{
	RectangleToCircle(manifold);
	manifold->normal = -manifold->normal;
}

void RectangleToCircle(Manifold* manifold)
{
	const entity::Rectangle* rectangleA = reinterpret_cast<const entity::Rectangle*>(manifold->entityA);
	const entity::Circle* circleB = reinterpret_cast<const entity::Circle*>(manifold->entityB);

	const Vector2& deltaPos = *circleB->GetPosition() - *rectangleA->GetPosition();

	Vector2 closest = deltaPos;

	float halfXExtent = rectangleA->GetWidth() / 2.0f;
	float halfYExtent = rectangleA->GetHeight() / 2.0f;

	closest.x = CLAMP(closest.x, -halfXExtent, halfXExtent);
	closest.y = CLAMP(closest.y, -halfYExtent, halfYExtent);

	bool inside = false;

	// Circle is inside the AABB, so we need to clamp the circle's center to the closest edge
	if (deltaPos == closest)
	{
		inside = true;

		if (abs(deltaPos.x) > abs(deltaPos.y))
		{
			closest.x = closest.x > 0 ? halfXExtent : -halfXExtent;
		}
		else
		{
			closest.y = closest.y > 0 ? halfYExtent : -halfYExtent;
		}
	}

	const Vector2& normal = deltaPos - closest;
	float normalLenSq = normal.LengthSq();
	float radius = circleB->GetRadius();

	// Early out of the radius is shorter than distance to closest point and
	// circle not inside the AABB
	if (normalLenSq > radius * radius && !inside)
	{
		return;
	}

	float normalLen = sqrt(normalLenSq);

	// Collision normal needs to be flipped to point outside if circle was inside the AABB
	manifold->normal = inside ? -normal : normal;
	manifold->penetration = radius - normalLen;
}

void RectangleToRectangle(Manifold* manifold)
{
	const entity::Rectangle* rectangleA = reinterpret_cast<const entity::Rectangle*>(manifold->entityA);
	const entity::Rectangle* rectangleB = reinterpret_cast<const entity::Rectangle*>(manifold->entityB);

	const Vector2& deltaPos = *rectangleB->GetPosition() - *rectangleA->GetPosition();

	float halfXExtentA = rectangleA->GetWidth() / 2.0f;
	float halfXExtentB = rectangleB->GetWidth() / 2.0f;
	float xOverlap = halfXExtentA + halfXExtentB - abs(deltaPos.x);
	if (xOverlap > 0)
	{
		float halfYExtentA = rectangleA->GetHeight() / 2.0f;
		float halfYExtentB = rectangleB->GetHeight() / 2.0f;
		float yOverlap = halfYExtentA + halfYExtentB - abs(deltaPos.y);
		if (yOverlap > 0)
		{
			if (xOverlap > yOverlap)
			{
				manifold->normal = deltaPos.x < 0 ? Vector2(-1, 0) : Vector2(1, 0);
				manifold->penetration = xOverlap;
			}
			else
			{
				manifold->normal = deltaPos.y < 0 ? Vector2(0, -1) : Vector2(0, 1);
				manifold->penetration = yOverlap;
			}
		}
	}
}

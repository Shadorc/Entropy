#include "Precompiled.h"

CollisionCallback Dispatch[(int) EntityType::COUNT][(int) EntityType::COUNT] =
{
  {
	CircleToCircle, CircleToRectangle
  },
  {
	RectangleToCircle, RectangleToRectangle
  },
};

Collision Solve(Entity* entityA, Entity* entityB)
{
	Collision collision(entityA, entityB);
	Dispatch[(int) entityA->GetType()][(int) entityB->GetType()](collision);
	return collision;
}

void CircleToCircle(Collision& collision)
{
	const entity::Circle* circleA = dynamic_cast<const entity::Circle*>(collision.entityA);
	const entity::Circle* circleB = dynamic_cast<const entity::Circle*>(collision.entityB);

	const Vector2& deltaPos = circleB->position - circleA->position;
	float deltaLenSq = deltaPos.LengthSq();
	float sumRadius = circleA->GetRadius() + circleB->GetRadius();

	if (deltaLenSq >= sumRadius * sumRadius)
	{
		collision.contactCount = 0;
		return;
	}

	collision.contactCount = 1;

	if (IsZero(deltaLenSq))
	{
		collision.penetration = circleA->GetRadius();
		collision.normal = Vector2(1, 0);
		collision.contacts.push_back(circleA->position);
	}
	else
	{
		float deltaLen = sqrtf(deltaLenSq);
		collision.penetration = sumRadius - deltaLen;
		collision.normal = deltaPos / deltaLen;
		collision.contacts.push_back(collision.normal * circleA->GetRadius() + circleA->position);
	}
}

void CircleToRectangle(Collision& collision)
{
	std::swap(collision.entityA, collision.entityB);
	RectangleToCircle(collision);
	collision.normal = -collision.normal;
}

void RectangleToCircle(Collision& collision)
{
	const entity::Rectangle* rectangleA = dynamic_cast<const entity::Rectangle*>(collision.entityA);
	const entity::Circle* circleB = dynamic_cast<const entity::Circle*>(collision.entityB);

	const Vector2& deltaPos = circleB->position - rectangleA->position;
	if (IsZero(deltaPos.LengthSq()))
	{
		collision.penetration = rectangleA->GetHeight() / 2.0f + circleB->GetRadius();
		collision.normal = Vector2(0, -1);
	}
	else
	{
		Vector2 closest = deltaPos;

		float halfWidth = rectangleA->GetWidth() / 2.0f;
		float halfHeight = rectangleA->GetHeight() / 2.0f;

		closest.x = Clamp(closest.x, -halfWidth, halfWidth);
		closest.y = Clamp(closest.y, -halfHeight, halfHeight);

		bool inside = false;

		// Circle is inside the AABB, so we need to clamp the circle's center to the closest edge
		if (deltaPos == closest)
		{
			inside = true;

			if (abs(deltaPos.x) > abs(deltaPos.y))
			{
				closest.x = closest.x > 0 ? halfWidth : -halfWidth;
			}
			else
			{
				closest.y = closest.y > 0 ? halfHeight : -halfHeight;
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

		float normalLen = sqrtf(normalLenSq);

		// Collision normal needs to be flipped to point outside if circle was inside the AABB
		collision.normal = (inside ? -normal : normal) / normalLen;
		collision.penetration = inside ? (radius + normalLen) : (radius - normalLen);
	}
}

void RectangleToRectangle(Collision& collision)
{
	const entity::Rectangle* rectangleA = dynamic_cast<const entity::Rectangle*>(collision.entityA);
	const entity::Rectangle* rectangleB = dynamic_cast<const entity::Rectangle*>(collision.entityB);

	const Vector2& deltaPos = rectangleB->position - rectangleA->position;

	float halfWidthA = rectangleA->GetWidth() / 2.0f;
	float halfWidthB = rectangleB->GetWidth() / 2.0f;
	float overlapX = halfWidthA + halfWidthB - abs(deltaPos.x);
	if (overlapX > 0)
	{
		float halfHeightA = rectangleA->GetHeight() / 2.0f;
		float halfHeightB = rectangleB->GetHeight() / 2.0f;
		float yOverlap = halfHeightA + halfHeightB - abs(deltaPos.y);
		if (yOverlap > 0)
		{
			if (overlapX < yOverlap)
			{
				collision.normal = deltaPos.x < 0 ? Vector2(-1, 0) : Vector2(1, 0);
				collision.penetration = overlapX;
			}
			else
			{
				collision.normal = deltaPos.y < 0 ? Vector2(0, -1) : Vector2(0, 1);
				collision.penetration = yOverlap;
			}
		}
	}
}

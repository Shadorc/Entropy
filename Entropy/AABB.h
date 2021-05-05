#pragma once

#include "Vector2.h"

class AABB
{
private:
	const Vector2* m_topLeft;
	const Vector2* m_bottomRight;

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
};


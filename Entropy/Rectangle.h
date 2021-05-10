#pragma once

#include "Entity.h"

namespace entity
{
	class Circle;
	class Rectangle : public Entity
	{
	private:
		float m_width;
		float m_height;

	public:
		Rectangle(float x, float y, float width, float height);

		void Paint() const override;
		AABB GetAABB() const override;

		bool Intersects(Vector2* point) const;
		bool Intersects(const Rectangle* rectangle) const;
		bool Intersects(const Circle* circle) const;

		Vector2 ComputePenetrationVector(const Rectangle* rectangle) const;
		Vector2 ComputePenetrationVector(const Circle* circle) const;
	};
}

#pragma once

#include "Entity.h"

namespace entity
{
	class Rectangle;
	class Circle : public Entity
	{
		private:
			float m_radius;

		public:
			Circle(float x, float y, float radius);

			float GetRadius() const;
			AABB GetAABB() const override;

			void Paint() const;

			bool Intersects(const Vector2* point) const;
			bool Intersects(const Rectangle* rectangle) const;
			bool Intersects(const Circle* rectangle) const;

			Vector2 ComputePenetrationVector(const Rectangle* rectangle) const;
			Vector2 ComputePenetrationVector(const Circle* circle) const;
	};
}

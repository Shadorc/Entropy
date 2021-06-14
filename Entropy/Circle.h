#pragma once

#include "Entity.h"

namespace entity
{
	class Circle : public Entity
	{
		private:
			float m_radius;

		public:
			Circle(float x, float y, float radius);

			float GetRadius() const;
			AABB GetAABB() const override;
			EntityType GetType() const override;

			void Rotate(float angle) override;

			void Paint() const;
	};
}

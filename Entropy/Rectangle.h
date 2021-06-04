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

		float GetWidth() const;
		float GetHeight() const;
		AABB GetAABB() const override;
		EntityType GetType() const override;

		void Paint() const override;
	};
}

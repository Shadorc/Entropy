#pragma once

#include "Entity.h"

namespace entity 
{
	class Rectangle : public Entity
	{
	private:
		float m_width;
		float m_height;

	public:
		Rectangle(float x, float y, float width, float height);

		void Paint() const override;
	};
}

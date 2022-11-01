#pragma once

#include "Entity/Entity.h"

namespace entity
{
	class Circle : public Entity
	{
	private:
		float m_Radius;

	public:
		Circle(float x, float y, float radius);

		float GetRadius() const;
		EntityType GetType() const override;

		std::unique_ptr<AABB> ComputeAABB() const override;

		void Rotate(float angle) override;
		void Paint() const override;
	};
}

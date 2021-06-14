#pragma once

#include "Entity.h"
#include "Vector2.h"
#include <array>

namespace entity
{
	class Polygon : public Entity
	{
		private:
			Matrix22 m_orientationMatrix;
			std::array<Vector2, MAX_POLYGON_VERTICES> m_vertices;
			std::array<Vector2, MAX_POLYGON_VERTICES> m_normals;

		public:
			Polygon(float x, float y);

			Matrix22 GetOrientationMatrix() const;
			unsigned int GetVertexCount() const;
			Vector2 GetVertex(unsigned int index) const;
			Vector2 GetNormal(unsigned int index) const;
			AABB GetAABB() const override;
			EntityType GetType() const override;

			Vector2 GetSupport(const Vector2& dir) const;

			void Paint() const;
	};
}

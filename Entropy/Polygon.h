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
			std::vector<Vector2> m_vertices;
			std::vector<Vector2> m_normals;

		public:
			Polygon(float x, float y);

			Matrix22 GetOrientationMatrix() const;
			size_t GetVertexCount() const;
			Vector2 GetVertex(size_t index) const;
			Vector2 GetNormal(size_t index) const;
			AABB GetAABB() const override;
			EntityType GetType() const override;

			Vector2 GetSupport(const Vector2& dir) const;

			// TODO
			void SetBox(float halfW, float halfH);

			void Paint() const;
	};
}

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
			// Rectangle
			Polygon(float x, float y, float width, float height);
			Polygon(float x, float y, std::vector<Vector2>& vertices);

			Matrix22 GetOrientationMatrix() const;
			size_t GetVertexCount() const;
			Vector2 GetVertex(size_t index) const;
			Vector2 GetNormal(size_t index) const;
			EntityType GetType() const override;

			Vector2 GetSupport(const Vector2& dir) const;

			AABB* ComputeAABB() const override;

			void Rotate(float angle) override;

			void Paint() const;
	};
}

#pragma once

#include "Entity.h"
#include "Vector2.h"
#include <array>

namespace entity
{
	class Polygon : public Entity
	{
		private:
			unsigned int m_vertexCount;
			std::array<Vector2, MAX_POLYGON_VERTICES> m_vertices;
			std::array<Vector2, MAX_POLYGON_VERTICES> m_normals;

		public:
			Polygon(float x, float y);

			Vector2 GetVertex(unsigned int index) const;
			unsigned int GetVertexCount() const;
			AABB GetAABB() const override;
			EntityType GetType() const override;

			Vector2 GetSupport(const Vector2& dir) const;

			void Paint() const;
	};
}
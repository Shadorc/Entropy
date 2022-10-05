#pragma once

#include "Entity/Entity.h"
#include "Math/Vector2.h"
#include <array>

namespace entity
{
	class Polygon : public Entity
	{
	private:
		Matrix22 m_OrientationMatrix;
		std::vector<Vector2> m_Vertices;
		std::vector<Vector2> m_Normals;

	public:
		// Rectangle
		Polygon(float x, float y, float width, float height);
		Polygon(float x, float y, std::vector<Vector2>& vertices);
		virtual ~Polygon();

		Matrix22 GetOrientationMatrix() const;
		size_t GetVertexCount() const;
		Vector2 GetVertex(size_t index) const;
		Vector2 GetNormal(size_t index) const;
		Vector2 GetSupport(const Vector2& dir) const;
		EntityType GetType() const override;

		AABB* ComputeAABB() const override;

		void Rotate(float angle) override;
		void Paint() const override;
	};
}

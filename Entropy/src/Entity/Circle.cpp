#include "Precompiled.h"

entity::Circle::Circle(float x, float y, float radius)
	: Entity(x, y)
	, m_Radius(radius)
{
	ENTROPY_ASSERT_WITH_REASON(radius > 0.0f, "Radius must be positive");
}

float entity::Circle::GetRadius() const
{
	return m_Radius;
}

EntityType entity::Circle::GetType() const
{
	return EntityType::CIRCLE;
}

std::unique_ptr<AABB> entity::Circle::ComputeAABB() const
{
	return std::make_unique<AABB>(
		m_Position.x - m_Radius, m_Position.y - m_Radius,
		m_Position.x + m_Radius, m_Position.y + m_Radius
	);
}

void entity::Circle::Rotate(float angle)
{
	m_Orientation += angle;
}

void entity::Circle::Paint() const
{
	RenderCircle(m_Position.x, m_Position.y, m_Radius);

	// Render line within circle so orientation is visible
	Vector2 radius = Vector2(-sinf(m_Orientation), cosf(m_Orientation));
	radius *= m_Radius;
	radius += m_Position;
	RenderLine(m_Position.x, m_Position.y, radius.x, radius.y);
}

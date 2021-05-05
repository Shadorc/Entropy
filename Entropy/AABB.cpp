#include "AABB.h"

AABB::AABB(Vector2 topLeft, Vector2 bottomRight) :
	m_topLeft(&topLeft),
	m_bottomRight(&bottomRight)
{

}

AABB::AABB(Vector2* topLeft, Vector2* bottomRight) :
	m_topLeft(topLeft),
	m_bottomRight(bottomRight)
{

}

AABB::~AABB() 
{
	delete m_topLeft;
	delete m_bottomRight;
}

float AABB::GetX() const
{
	return m_topLeft->m_x;
}

float AABB::GetY() const
{
	return m_topLeft->m_y;
}

float AABB::GetWidth() const
{
	return m_bottomRight->m_x - m_topLeft->m_x;
}

float AABB::GetHeight() const
{
	return m_bottomRight->m_y - m_topLeft->m_y;
}

const Vector2* AABB::GetTopLeft() const
{
	return m_topLeft;
}

const Vector2* AABB::GetBottomRight() const
{
	return m_bottomRight;
}

bool AABB::IntersectsWith(const AABB& other) const
{
	AABB md = other.MinkowskiDifference(*this);
	return md.m_topLeft->m_x <= 0 && md.m_bottomRight->m_x >= 0
		&& md.m_topLeft->m_y <= 0 && md.m_bottomRight->m_y >= 0;
}

AABB AABB::MinkowskiDifference(const AABB& other) const
{
	Vector2 topLeft = *m_topLeft - *other.m_bottomRight;
	Vector2 bottomRight = *m_bottomRight - *other.m_topLeft;
	return AABB(topLeft, bottomRight);
}
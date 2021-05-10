#include "Vector2.h"
#include "Config.h"
#include <math.h>

const float Vector2::EPSILON = (float) pow(10, -4);
const float Vector2::EPSILON_SQ = (float) pow(EPSILON, 2);
const Vector2 Vector2::ZERO = Vector2(0, 0);

Vector2::Vector2() :
	Vector2(0, 0)
{

}

Vector2::Vector2(float x, float y) :
	m_x(x),
	m_y(y)
{

}

Vector2 Vector2::ToNormalizedSpace()
{
	return Vector2::ToNormalizedSpace(m_x, m_y);
}

void Vector2::Normalize()
{
	float length = Length();
	if (length < EPSILON)
	{
		m_x = 0;
		m_y = 0;
	}
	else
	{
		*this /= length;
	}
}

float Vector2::Dot(const Vector2& other) const
{
	return m_x * other.m_x + m_y * other.m_y;
}

float Vector2::Cross(const Vector2& other) const 
{
	return m_x * other.m_y - m_y * other.m_x;
}

Vector2 Vector2::Tangent() const
{
	return Vector2(-m_y, m_x);
}

float Vector2::Length() const
{
	return (float) sqrt(LengthSq());
}

float Vector2::LengthSq() const
{
	return Dot(*this);
}

float Vector2::Distance(const Vector2& other) const
{
	return (float) sqrt(DistanceSq(other));
}

float Vector2::DistanceSq(const Vector2& other) const
{
	return (float) (pow(static_cast<double>(m_x) - other.m_x, 2) + pow(static_cast<double>(m_y) - other.m_y, 2));
}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return Vector2(m_x + other.m_x, m_y + other.m_y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return Vector2(m_x - other.m_x, m_y - other.m_y);
}

Vector2 Vector2::operator*(float coef) const
{
	return Vector2(m_x * coef, m_y * coef);
}

Vector2 Vector2::operator/(float coef) const
{
	return Vector2(m_x / coef, m_y / coef);
}

void Vector2::operator+=(const Vector2& other)
{
	m_x += other.m_x;
	m_y += other.m_y;
}

void Vector2::operator-=(const Vector2& other)
{
	m_x -= other.m_x;
	m_y -= other.m_y;
}

void Vector2::operator*=(float coef)
{
	m_x *= coef;
	m_y *= coef;
}

void Vector2::operator/=(float coef)
{
	m_x /= coef;
	m_y /= coef;
}

bool Vector2::operator==(const Vector2& other) const
{
	return m_x == other.m_x && m_y == other.m_y;
}

Vector2 Vector2::ToNormalizedSpace(float x, float y)
{
	return Vector2(2 * x / WIDTH - 1, 1 - 2 * y / HEIGHT);
}

#include "Precompiled.h"

Matrix22::Matrix22()
	: Matrix22(0.0f, 0.0f, 0.0f, 0.0f)
{
}

Matrix22::Matrix22(float m00, float m01, float m10, float m11)
	: m00(m00)
	, m01(m01)
	, m10(m10)
	, m11(m11)
{
}

Matrix22::Matrix22(Vector2& columnX, Vector2& columnY)
	: columnX(columnX)
	, columnY(columnY)
{
}

Matrix22::Matrix22(float angle)
{
	float cos = cosf(angle);
	float sin = sinf(angle);
	m00 = cos;
	m01 = -sin;
	m10 = sin;
	m11 = cos;
}

void Matrix22::Reset()
{
	m00 = 0.0f;
	m01 = 0.0f;
	m10 = 0.0f;
	m11 = 0.0f;
}

Matrix22 Matrix22::Transpose() const
{
	return Matrix22(m00, m10, m01, m11);
}

Matrix22 Matrix22::operator+(const Matrix22& other) const
{
	return Matrix22(
		m00 + other.m00, 
		m01 + other.m01, 
		m10 + other.m10, 
		m11 + other.m11
	);
}

Matrix22 Matrix22::operator*(const Matrix22& other) const
{
	return Matrix22(
		m00 * other.m00 + m01 * other.m10,
		m00 * other.m01 + m01 * other.m11,
		m10 * other.m00 + m11 * other.m10,
		m10 * other.m01 + m11 * other.m11
	);
}

Vector2 Matrix22::operator*(const Vector2& vector) const
{
	return Vector2(
		m00 * vector.x + m01 * vector.y,
		m10 * vector.x + m11 * vector.y
	);
}

#include "Precompiled.h"

const Vector2 Vector2::ZERO = Vector2(0, 0);

Vector2::Vector2()
	: Vector2(0, 0)
{

}

Vector2::Vector2(float x, float y)
	: x(x)
	, y(y)
{

}

Vector2::Vector2(int x, int y)
	: Vector2(static_cast<float>(x), static_cast<float>(y))
{

}

void Vector2::Reset()
{
	x = 0.0f;
	y = 0.0f;
}

void Vector2::Normalize()
{
	float length = Length();
	if (IsZero(length))
	{
		Reset();
	}
	else
	{
		*this /= length;
	}
}

float Vector2::Dot(const Vector2& other) const
{
	return x * other.x + y * other.y;
}

float Vector2::Cross(const Vector2& other) const
{
	return x * other.y - y * other.x;
}

Vector2 Vector2::Tangent() const
{
	return Vector2(-y, x);
}

float Vector2::Length() const
{
	return sqrtf(LengthSq());
}

float Vector2::LengthSq() const
{
	return Dot(*this);
}

float Vector2::Distance(const Vector2& other) const
{
	return sqrtf(DistanceSq(other));
}

float Vector2::DistanceSq(const Vector2& other) const
{
	return powf(x - other.x, 2.0f) + powf(y - other.y, 2.0f);
}

std::string Vector2::ToString() const
{
	return "{" + std::to_string(x) + ", " + std::to_string(y) + "}";
}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(float scalar) const
{
	return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/(float scalar) const
{
	return Vector2(x / scalar, y / scalar);
}

Vector2 operator*(float scalar, const Vector2& vector)
{
	return vector * scalar;
}

Vector2 operator/(float scalar, const Vector2& vector)
{
	return vector / scalar;
}

void Vector2::operator+=(const Vector2& other)
{
	x += other.x;
	y += other.y;
}

void Vector2::operator-=(const Vector2& other)
{
	x -= other.x;
	y -= other.y;
}

void Vector2::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
}

void Vector2::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
}

bool Vector2::operator==(const Vector2& other) const
{
	return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2& other) const
{
	return x != other.x || y != other.y;
}

std::ostream& operator<<(std::ostream& os, const Vector2& vector)
{
	return os << vector.ToString();
}

Vector2 Vector2::Cross(const Vector2& vector, float scalar)
{
	return Vector2(scalar * vector.y, -scalar * vector.x);
}

Vector2 Vector2::Cross(float scalar, const Vector2& vector)
{
	return Vector2(-scalar * vector.y, scalar * vector.x);
}
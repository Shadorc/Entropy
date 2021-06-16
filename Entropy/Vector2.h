#pragma once

struct Vector2
{
	static const Vector2 ZERO;

	float x;
	float y;

	Vector2();
	Vector2(float x, float y);

	void Reset();
	void Normalize();

	float Dot(const Vector2& other) const;
	float Cross(const Vector2& other) const;
	Vector2 Tangent() const;
	float Length() const;
	float LengthSq() const;
	float Distance(const Vector2& other) const;
	float DistanceSq(const Vector2& other) const;

	std::string ToString() const;

	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-() const;
	Vector2 operator-(const Vector2& other) const;
	Vector2 operator*(float scalar) const;
	Vector2 operator/(float scalar) const;

	friend Vector2 operator*(float scalar, const Vector2& other);
	friend Vector2 operator/(float scalar, const Vector2& other);

	void operator+=(const Vector2& other);
	void operator-=(const Vector2& other);
	void operator*=(float scalar);
	void operator/=(float scalar);
	bool operator==(const Vector2& other) const;

	static Vector2 Cross(const Vector2& vector, float scalar);
	static Vector2 Cross(float scalar, const Vector2& vector);
};

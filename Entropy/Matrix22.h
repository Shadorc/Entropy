#pragma once

#include "Vector2.h"

struct Matrix22
{
	static const Matrix22 ZERO;

	struct
	{
		float m00, m01;
		float m10, m11;
	};

	Matrix22();
	Matrix22(float m00, float m01, float m10, float m11);
	Matrix22(float angle);

	void Reset();
	void FromAngle(float angle);

	Matrix22 Transpose() const;

	Matrix22 operator+(const Matrix22& other) const;
	Matrix22 operator*(const Matrix22& other) const;
	Vector2 operator*(const Vector2& vector) const;
};

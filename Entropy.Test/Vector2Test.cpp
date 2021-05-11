#include "pch.h"
#include "../Entropy/Vector2.cpp"

TEST(Vector2Construcor, Vector2Test)
{
	Vector2 vector(2, 1);
	EXPECT_EQ(vector.m_x, 2);
	EXPECT_EQ(vector.m_y, 1);
}

TEST(Vector2Equal, Vector2Test)
{
	EXPECT_TRUE(Vector2(1, 1) == Vector2(1.0f, 1.0f));
}

TEST(Vector2Sum, Vector2Test)
{
	Vector2 vector1(2, 1);
	Vector2 vector2(5, -4);
	Vector2 vector3 = vector1 + vector2;
	EXPECT_EQ(vector1, Vector2(2, 1));
	EXPECT_EQ(vector2, Vector2(5, -4));
	EXPECT_EQ(vector3, Vector2(7, -3));
}

TEST(Vector2Subtract, Vector2Test)
{
	Vector2 vector1(2, 1);
	Vector2 vector2(5, -4);
	Vector2 vector3 = vector1 - vector2;
	EXPECT_EQ(vector1, Vector2(2, 1));
	EXPECT_EQ(vector2, Vector2(5, -4));
	EXPECT_EQ(vector3, Vector2(-3, 5));
}

TEST(Vector2Multiply, Vector2Test)
{
	Vector2 vector1(2, 1);
	Vector2 vector2 = vector1 * 2.0f;
	EXPECT_EQ(vector1, Vector2(2, 1));
	EXPECT_EQ(vector2, Vector2(4, 2));
}

TEST(Vector2Divide, Vector2Test)
{
	Vector2 vector1(2, 1);
	Vector2 vector2 = vector1 / 2.0f;
	EXPECT_EQ(vector1, Vector2(2, 1));
	EXPECT_EQ(vector2, Vector2(1, 0.5f));
}

TEST(Vector2SumEqual, Vector2Test)
{
	Vector2 vector1(2, 1);
	Vector2 vector2(5, -4);
	vector1 += vector2;
	EXPECT_EQ(vector1, Vector2(7, -3));
	EXPECT_EQ(vector2, Vector2(5, -4));
}

TEST(Vector2SubtractEqual, Vector2Test)
{
	Vector2 vector1(2, 1);
	Vector2 vector2(5, -4);
	vector1 -= vector2;
	EXPECT_EQ(vector1, Vector2(-3, 5));
	EXPECT_EQ(vector2, Vector2(5, -4));
}

TEST(Vector2MultiplyEqual, Vector2Test)
{
	Vector2 vector1(2, 1);
	vector1 *= 2.0f;
	EXPECT_EQ(vector1, Vector2(4, 2));
}

TEST(Vector2DivideEqual, Vector2Test)
{
	Vector2 vector1(2, 1);
	vector1 /= 2.0f;
	EXPECT_EQ(vector1, Vector2(1, 0.5f));
}

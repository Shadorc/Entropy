#include "pch.h"
#include "../Entropy/Vector2.cpp"

TEST(Vector2EmptyConstrucor, Vector2Test)
{
	Vector2 vector;
	EXPECT_EQ(vector.x, 0);
	EXPECT_EQ(vector.y, 0);
}

TEST(Vector2Construcor, Vector2Test)
{
	Vector2 vector(2, 1);
	EXPECT_EQ(vector.x, 2);
	EXPECT_EQ(vector.y, 1);
}

TEST(Vector2Dot, Vector2Test)
{
	Vector2 vector1(-6, 8);
	Vector2 vector2(5, 12);
	float dot = vector1.Dot(vector2);
	EXPECT_EQ(vector1, Vector2(-6, 8));
	EXPECT_EQ(vector2, Vector2(5, 12));
	EXPECT_EQ(dot, 66);
}

TEST(Vector2Cross, Vector2Test)
{
	Vector2 vector1(5, 7);
	Vector2 vector2(13, 17);
	float dot = vector1.Cross(vector2);
	EXPECT_EQ(vector1, Vector2(5, 7));
	EXPECT_EQ(vector2, Vector2(13, 17));
	EXPECT_EQ(dot, -6);
}

TEST(Vector2Tangent, Vector2Test)
{
	Vector2 vector1(5, 7);
	Vector2 tangent = vector1.Tangent();
	EXPECT_EQ(vector1, Vector2(5, 7));
	EXPECT_EQ(tangent, Vector2(-7, 5));
}

TEST(Vector2Length, Vector2Test)
{
	Vector2 vector(3, -4);
	float length = vector.Length();
	EXPECT_EQ(vector, Vector2(3, -4));
	EXPECT_EQ(length, 5);
}

TEST(Vector2LengthSq, Vector2Test)
{
	Vector2 vector(3, -4);
	float length = vector.LengthSq();
	EXPECT_EQ(vector, Vector2(3, -4));
	EXPECT_EQ(length, 25);
}

TEST(Vector2Distance, Vector2Test)
{
	Vector2 vector1(3, -4);
	Vector2 vector2(5, -4);
	float distance = vector1.Distance(vector2);
	EXPECT_EQ(vector1, Vector2(3, -4));
	EXPECT_EQ(vector2, Vector2(5, -4));
	EXPECT_EQ(distance, 2);
}

TEST(Vector2DistanceSq, Vector2Test)
{
	Vector2 vector1(3, -4);
	Vector2 vector2(5, -4);
	float distanceSq = vector1.DistanceSq(vector2);
	EXPECT_EQ(vector1, Vector2(3, -4));
	EXPECT_EQ(vector2, Vector2(5, -4));
	EXPECT_EQ(distanceSq, 4);
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

TEST(Vector2Equal, Vector2Test)
{
	EXPECT_TRUE(Vector2(1, 1) == Vector2(1.0f, 1.0f));
}
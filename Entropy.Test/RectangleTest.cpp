#include "pch.h"
#include "../Entropy/Entity.cpp"
#include "../Entropy/Rectangle.cpp"
#include "../Entropy/Circle.cpp"
#include "../Entropy/AABB.cpp"

TEST(RectangleConstrucor, RectangleTest)
{
	entity::Rectangle rectangle(0, 0, 100, 100);
	EXPECT_EQ(rectangle.GetPosition()->m_x, 0);
	EXPECT_EQ(rectangle.GetPosition()->m_y, 0);
	EXPECT_EQ(rectangle.GetWidth(), 100.0f);
	EXPECT_EQ(rectangle.GetHeight(), 100.0f);
}

TEST(RectangleGetAABB, RectangleTest)
{
	entity::Rectangle rectangle(0, 0, 100, 100);
	AABB aabb = rectangle.GetAABB();
	EXPECT_EQ(aabb.GetWidth(), rectangle.GetWidth());
	EXPECT_EQ(aabb.GetHeight(), rectangle.GetHeight());
	EXPECT_EQ(*aabb.GetBottomRight(), Vector2(50, 50));
	EXPECT_EQ(*aabb.GetTopLeft(), Vector2(-50, -50));
	EXPECT_EQ(aabb.GetX(), -50);
	EXPECT_EQ(aabb.GetY(), -50);
}

TEST(RectangleIntersectsVector2, RectangleTest)
{
	entity::Rectangle rectangle(0, 0, 100, 100);
	Vector2 inside(0, 0);
	EXPECT_TRUE(rectangle.Intersects(&inside));
	Vector2 border(-50, -50);
	EXPECT_TRUE(rectangle.Intersects(&border));
	Vector2 outside(100, 100);
	EXPECT_FALSE(rectangle.Intersects(&outside));
}

TEST(RectangleIntersectsRectangle, RectangleTest)
{
	entity::Rectangle rectangle(0, 0, 100, 100);
	entity::Rectangle smaller(0, 0, 50, 50);
	EXPECT_TRUE(rectangle.Intersects(&smaller));
	entity::Rectangle bigger(0, 0, 200, 200);
	EXPECT_TRUE(rectangle.Intersects(&bigger));
	entity::Rectangle border(-50, 0, 100, 100);
	EXPECT_TRUE(rectangle.Intersects(&border));
	entity::Rectangle outside(100, 100, 100, 100);
	EXPECT_FALSE(rectangle.Intersects(&outside));
}

TEST(RectangleIntersectsCircle, RectangleTest)
{
	entity::Rectangle rectangle(0, 0, 100, 100);
	entity::Circle smaller(0, 0, 25);
	EXPECT_TRUE(rectangle.Intersects(&smaller));
	entity::Circle bigger(0, 0, 50);
	EXPECT_TRUE(rectangle.Intersects(&bigger));
	entity::Circle border(-50, 0, 25);
	EXPECT_TRUE(rectangle.Intersects(&border));
	entity::Circle outside(100, 100, 25);
	EXPECT_FALSE(rectangle.Intersects(&outside));
}
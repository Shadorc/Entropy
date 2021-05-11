#include "pch.h"
#include "../Entropy/AABB.h"
#include "../Entropy/Circle.h"
#include "../Entropy/Rectangle.h"

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
	EXPECT_EQ(aabb.GetX(), rectangle.GetPosition()->m_x);
	EXPECT_EQ(aabb.GetY(), rectangle.GetPosition()->m_y);
}
#include "pch.h"
#include "../Entropy/Vector2.h"

TEST(Vector2AreEqual, Vector2Test) {
	EXPECT_TRUE(Vector2(1, 1) == Vector2(1.0f, 1.0f));
}
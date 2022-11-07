#pragma once

#include <vector>
#include "Math/Vector2.h"

struct Edge
{
	Vector2 start;
	Vector2 end;
};

Vector2 Raycast(const Vector2& origin, const Vector2& direction, const std::vector<Edge>& edges);

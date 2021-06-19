#pragma once

#include <vector>

#include "Vector2.h"
#include "MathIE.h"

struct Edge
{
	Vector2 start;
	Vector2 end;
};

Vector2 Raycast(const Vector2& origin, const Vector2& direction, std::vector<Edge>& edges);

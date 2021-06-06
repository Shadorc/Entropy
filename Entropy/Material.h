#pragma once

struct Material
{
	float density;
	float restitution;
};

constexpr Material ROCK = { 0.6f, 0.1f };
constexpr Material WOOD = { 0.3f, 0.2f };
constexpr Material METAL = { 1.2f, 0.05f };
constexpr Material BOUNCY_BALL = { 0.3f, 0.8f };
constexpr Material SUPER_BALL = { 0.3f, 0.95f };
constexpr Material PILLOW = { 0.1f, 0.2f };
constexpr Material STATIC = { 0.0f, 0.4f };
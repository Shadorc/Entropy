#pragma once

struct Material
{
	float density;
	float restitution;
};

constexpr Material MATERIAL_ROCK = { 0.6f, 0.1f };
constexpr Material MATERIAL_WOOD = { 0.3f, 0.2f };
constexpr Material MATERIAL_METAL = { 1.2f, 0.05f };
constexpr Material MATERIAL_BOUNCY_BALL = { 0.3f, 0.8f };
constexpr Material MATERIAL_SUPER_BALL = { 0.3f, 0.95f };
constexpr Material MATERIAL_PILLOW = { 0.1f, 0.2f };
constexpr Material MATERIAL_STATIC = { 0.0f, 0.4f };
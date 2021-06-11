#pragma once

struct MaterialData
{
	float density;
	float restitution;
};

constexpr MaterialData MATERIAL_ROCK = { 0.6f, 0.1f };
constexpr MaterialData MATERIAL_WOOD = { 0.3f, 0.2f };
constexpr MaterialData MATERIAL_METAL = { 1.2f, 0.05f };
constexpr MaterialData MATERIAL_BOUNCY_BALL = { 0.3f, 0.8f };
constexpr MaterialData MATERIAL_SUPER_BALL = { 0.3f, 0.95f };
constexpr MaterialData MATERIAL_PILLOW = { 0.1f, 0.2f };
constexpr MaterialData MATERIAL_STATIC = { 0.0f, 0.4f };
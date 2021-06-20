#pragma once

struct FrictionData
{
	float staticFactor;
	float dynamicFactor;
};

constexpr FrictionData FRICTION_NONE = { 0.0f, 0.0f };
constexpr FrictionData FRICTION_LOW = { 0.1f, 0.05f };
constexpr FrictionData FRICTION_NORMAL = { 0.2f, 0.1f };
constexpr FrictionData FRICTION_HIGH = { 0.5f, 0.25f };
constexpr FrictionData FRICTION_MAXIMUM = { 1.0f, 1.0f };
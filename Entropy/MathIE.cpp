#include "Precompiled.h"

int Rand(int max)
{
	return rand() % max + 1;
}

int Rand(int min, int max)
{
	return rand() % (max - min) + min + 1;
}

float Clamp(float value, float min, float max)
{
	return value < min ? min : (value > max ? max : value);
}

bool IsZero(float a)
{
	return abs(a) <= EPSILON;
}

bool Equal(float a, float b)
{
	return IsZero(a - b);
}

constexpr float k_biasRelative = 0.95f;
constexpr float k_biasAbsolute = 0.01f;
bool BiasGreaterThan(float a, float b)
{
	return a >= b * k_biasRelative + a * k_biasAbsolute;
}
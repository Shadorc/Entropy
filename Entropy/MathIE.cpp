#include "Precompiled.h"

float Pythagore(float a, float b)
{
	return sqrtf(a * a + b * b);
}

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
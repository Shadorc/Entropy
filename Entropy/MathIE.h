#pragma once

#include "Precompiled.h"

constexpr float FLOAT_INFINITY = std::numeric_limits<float>::infinity();
constexpr float EPSILON = 0.0001f;
constexpr float PI = 3.14159265f;
constexpr float TWICE_PI = 2.0f * PI;

float Pythagore(float a, float b);
int Rand(int max);
int Rand(int min, int max);
float Clamp(float value, float min, float max);
bool IsZero(float a);
bool Equal(float a, float b);
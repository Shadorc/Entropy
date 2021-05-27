#pragma once

#define FLOAT_INFINITY std::numeric_limits<float>::infinity()
#define RAND_INT(max) rand() % max + 1
#define INT(a) static_cast<int>(a)
#define CLAMP(value, min, max) (value < min ? min : (value > max ? max : value))
#define MIN(a, b) a < b ? a : b

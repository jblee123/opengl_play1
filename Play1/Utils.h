#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <stdlib.h>

extern const float RAD_PER_CIRCLE;

inline float randf() {
    return ((float)rand() / (float)RAND_MAX);
}

inline float randAngle() {
    return randf() * RAD_PER_CIRCLE;
}

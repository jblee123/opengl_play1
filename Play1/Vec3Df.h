#pragma once

#include "Vector.h"

namespace vec3df {

typedef vec::Vector<float, 3> Vec3Df;

Vec3Df create(float x, float y, float z);
Vec3Df create(float x, float y);
Vec3Df create(float x);
Vec3Df create();
Vec3Df& translate(Vec3Df& vec, float x, float y, float z);
Vec3Df& rotateX(Vec3Df& vec, float theta);
Vec3Df& rotateY(Vec3Df& vec, float theta);
Vec3Df& rotateZ(Vec3Df& vec, float theta);
Vec3Df cross(const Vec3Df& vec, const Vec3Df& rhs);

} // of namespace vec3df

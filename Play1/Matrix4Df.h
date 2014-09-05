#pragma once

#include "Matrix.h"
#include "Vec4Df.h"

#pragma once

namespace mat4df {

typedef mat::Matrix<float, 4> Mat4Df;

inline Mat4Df create(
    float f00, float f01, float f02, float f03,
    float f10, float f11, float f12, float f13,
    float f20, float f21, float f22, float f23,
    float f30, float f31, float f32, float f33) {
        
    Mat4Df mat;

    mat(0, 0) = f00;
    mat(0, 1) = f01;
    mat(0, 2) = f02;
    mat(0, 3) = f03;

    mat(1, 0) = f10;
    mat(1, 1) = f11;
    mat(1, 2) = f12;
    mat(1, 3) = f13;

    mat(2, 0) = f20;
    mat(2, 1) = f21;
    mat(2, 2) = f22;
    mat(2, 3) = f23;

    mat(3, 0) = f30;
    mat(3, 1) = f31;
    mat(3, 2) = f32;
    mat(3, 3) = f33;

    return mat;
}

inline vec4df::Vec4Df mul(const Mat4Df& mat, const vec4df::Vec4Df& vec) {
    vec4df::Vec4Df result;

    for (int row = 0; row < mat.size(); row++) {
        for (int col = 0; col < mat.size(); col++) {
            result(row) += mat(col, row) * vec(col);
        }
    }

    return result;
}

} // of namespace mat4df

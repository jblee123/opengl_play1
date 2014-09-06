#include "Matrix4Df.h"
#include "Vec3D.h"

namespace projection {

mat4df::Mat4Df createLookAt(
    const vec3df::Vec3Df& eye,
    const vec3df::Vec3Df& target,
    const vec3df::Vec3Df& up) {

    mat4df::Mat4Df mat;

    vec3df::Vec3Df fwd = (target - eye).getUnit();
    vec3df::Vec3Df side = vec3df::cross(fwd, up);
    vec3df::Vec3Df upPrime = vec3df::cross(side, fwd);

    mat(0, 0) = side(0);
    mat(0, 1) = side(1);
    mat(0, 2) = side(2);
    mat(0, 3) = 0;

    mat(1, 0) = upPrime(0);
    mat(1, 1) = upPrime(1);
    mat(1, 2) = upPrime(2);
    mat(1, 3) = 0;

    mat(2, 0) = fwd(0);
    mat(2, 1) = fwd(1);
    mat(2, 2) = fwd(2);
    mat(2, 3) = 0;

    mat(3, 0) = -eye(0);
    mat(3, 1) = -eye(1);
    mat(3, 2) = -eye(2);
    mat(3, 3) = 1;

    return mat;
}

mat4df::Mat4Df createPerspective(
    float left, float top, float right, float bottom, float near_dist, float far_dist) {

    mat4df::Mat4Df mat;

    mat(0, 0) = (2 * near_dist) / (right - left);

    mat(1, 1) = (2 * near_dist) / (top - bottom);

    mat(2, 0) = (right + left) / (right - left);
    mat(2, 1) = (top + bottom) / (top - bottom);
    mat(2, 2) = (near_dist + far_dist) / (near_dist - far_dist);
    mat(2, 3) = -1;

    mat(3, 2) = (2 * near_dist * far_dist) / (near_dist - far_dist);

    return mat;
}

mat4df::Mat4Df createOrtho(
    float left, float top, float right, float bottom, float near_dist, float far_dist) {

    mat4df::Mat4Df mat;

    mat(0, 0) = 2 / (right - left);

    mat(1, 1) = 2 / (top - bottom);

    mat(2, 2) = 2 / (near_dist - far_dist);

    mat(3, 0) = (left + right) / (left - right);
    mat(3, 1) = (bottom + top) / (bottom - top);
    mat(3, 2) = (near_dist + far_dist) / (near_dist - far_dist);
    mat(3, 3) = 1;

    return mat;
}

}

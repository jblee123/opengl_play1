#include <math.h>

#include "Vec3D.h"

Vec3D::Vec3D() :
    Vec3D(0, 0, 0) {
}

Vec3D::Vec3D(float x, float y) :
    Vec3D(x, y, 0) {
}

Vec3D::Vec3D(float x, float y, float z)
    : m_x(x), m_y(y), m_z(z) {
}

Vec3D::~Vec3D()
{
}

float Vec3D::getX() const { return m_x; }
float Vec3D::getY() const { return m_y; }
float Vec3D::getZ() const { return m_z; }

Vec3D& Vec3D::set(float x, float y) {
    return set(x, y, m_z);
}

Vec3D& Vec3D::set(float x, float y, float z) {
    m_x = x;
    m_y = y;
    m_z = z;
    return *this;
}

Vec3D& Vec3D::translate(float x, float y) {
    return translate(x, y, 0);
}

Vec3D& Vec3D::translate(float x, float y, float z) {
    m_x += x;
    m_y += y;
    m_z += z;
    return *this;
}

Vec3D& Vec3D::rotateX(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    float y = m_y;
    float z = m_z;
    m_y = y * c - z * s;
    m_z = y * s + z * c;
    return *this;
}

Vec3D& Vec3D::rotateY(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    float x = m_x;
    float z = m_z;
    m_x = x * c + z * s;
    m_z = -x * s + z * c;
    return *this;
}

Vec3D& Vec3D::rotateZ(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    float x = m_x;
    float y = m_y;
    m_x = x * c - y * s;
    m_y = x * s + y * c;
    return *this;
}

Vec3D Vec3D::operator+(const Vec3D& rhs) const {
    return Vec3D(
        m_x + rhs.m_x,
        m_y + rhs.m_y,
        m_z + rhs.m_z);
}

Vec3D Vec3D::operator-(const Vec3D& rhs) const {
    return Vec3D(
        m_x - rhs.m_x,
        m_y - rhs.m_y,
        m_z - rhs.m_z);
}

Vec3D Vec3D::operator*(float scale) const {
    return Vec3D(
        m_x * scale,
        m_y * scale,
        m_z * scale);
}

Vec3D Vec3D::operator/(float scale) const {
    return Vec3D(
        m_x / scale,
        m_y / scale,
        m_z / scale);
}

Vec3D Vec3D::cross(const Vec3D& rhs) const {
    return Vec3D(
        m_y * rhs.m_z - m_z * rhs.m_y,
        m_z * rhs.m_x - m_x * rhs.m_z,
        m_x * rhs.m_y - m_y * rhs.m_x);
}

float Vec3D::dot(const Vec3D& rhs) const {
    return (
        m_x * rhs.m_x +
        m_y * rhs.m_y +
        m_z * rhs.m_z);
}

float Vec3D::length() const {
    return sqrt(lengthSq());
}

float Vec3D::lengthSq() const {
    return m_x * m_x + m_y * m_y + m_z * m_z;
}

Vec3D Vec3D::getUnit() const {
    float len = length();
    return (len != 0) ? (*this / len) : Vec3D(1, 0, 0);
}

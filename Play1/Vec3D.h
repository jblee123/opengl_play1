#pragma once

class Vec3D
{
public:
    Vec3D();
    Vec3D(float x, float y);
    Vec3D(float x, float y, float z);
    virtual ~Vec3D();

    float getX() const;
    float getY() const;
    float getZ() const;

    Vec3D& set(float x, float y);
    Vec3D& set(float x, float y, float z);

    Vec3D& translate(float x, float y);
    Vec3D& translate(float x, float y, float z);

    Vec3D& rotateX(float theta);
    Vec3D& rotateY(float theta);
    Vec3D& rotateZ(float theta);

    Vec3D operator+(const Vec3D& rhs) const;
    Vec3D operator-(const Vec3D& rhs) const;
    Vec3D operator*(float scale) const;
    Vec3D operator/(float scale) const;

    Vec3D cross(const Vec3D& rhs) const;
    float dot(const Vec3D& rhs) const;

    float length() const;
    float lengthSq() const;

    Vec3D getUnit() const;

protected:
    float m_x;
    float m_y;
    float m_z;
};

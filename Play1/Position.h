#pragma once

#include "Vec3D.h"

class Position
{
public:
    Position();
    Position(const Vec3D& location);
    Position(const Vec3D& location, float heading);
    virtual ~Position();

    Vec3D getLocation() const;
    float getHeading() const;

protected:
    Vec3D m_location;
    float m_heading;
};

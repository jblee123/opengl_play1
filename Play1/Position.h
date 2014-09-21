#pragma once

#include "Vec3Df.h"

class Position
{
public:
    Position();
    Position(const vec3df::Vec3Df& location);
    Position(vec3df::Vec3Df location, float heading);
    virtual ~Position();

    vec3df::Vec3Df getLocation() const;
    float getHeading() const;
    
protected:
    vec3df::Vec3Df m_location;
    float m_heading;
};

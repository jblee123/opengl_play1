#include "Position.h"

Position::Position() :
Position(vec3df::create()) {
}

Position::Position(const vec3df::Vec3Df& location) :
    Position(location, 0) {
}

Position::Position(vec3df::Vec3Df location, float heading) {
    m_location = location;
    m_heading = heading;
}

Position::~Position() {
}

vec3df::Vec3Df Position::getLocation() const {
    return m_location;
}

float Position::getHeading() const {
    return m_heading;
}

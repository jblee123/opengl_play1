#include "Position.h"

Position::Position() :
    Position(Vec3D()) {
}

Position::Position(const Vec3D& location) :
    Position(location, 0) {
}

Position::Position(const Vec3D& location, float heading) {
    m_location = location;
    m_heading = heading;
}

Position::~Position() {
}

Vec3D Position::getLocation() const {
    return m_location;
}

float Position::getHeading() const {
    return m_heading;
}

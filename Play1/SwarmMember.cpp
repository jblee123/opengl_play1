#define _USE_MATH_DEFINES

#include <math.h>

#include "SwarmMember.h"

SwarmMember::SwarmMember(int id) :
    SwarmMember(id, Position()) {
}

SwarmMember::SwarmMember(int id, const Position& pos) {
    m_id = id;
    m_pos = pos;
    m_color = Color();
}

int SwarmMember::getId() const {
    return m_id;
}

Position SwarmMember::getPos() const {
    return m_pos;
}

Position SwarmMember::getPosForAnimation(DWORD time) const {
    float newHeading = fmod(time / 1000.f, (float)(2.0 * M_PI));
    newHeading += m_pos.getHeading();
    Vec3D offset(0.1, 0, 0);
    offset.rotateZ(newHeading);
    return Position(m_pos.getLocation() + offset, newHeading);
}

Color SwarmMember::getColor() const {
    return m_color;
}

void SwarmMember::setColor(Color color) {
    m_color = color;
}

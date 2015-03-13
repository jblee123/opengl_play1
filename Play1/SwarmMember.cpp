#include "SwarmMember.h"
#include "Utils.h"

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
    time %= 10000000;
    float newHeading = fmod(time / 1000.f, (float)(2.0 * PI));
    newHeading += m_pos.getHeading();
    vec3df::Vec3Df offset = vec3df::create(50, 0, 0);
    vec3df::rotateZ(offset, -newHeading);
    return Position(m_pos.getLocation() + offset, -newHeading);
}

Color SwarmMember::getColor() const {
    return m_color;
}

void SwarmMember::setColor(Color color) {
    m_color = color;
}

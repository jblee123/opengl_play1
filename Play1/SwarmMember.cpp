#include "SwarmMember.h"

SwarmMember::SwarmMember(int id) :
    SwarmMember(id, Position()) {
}

SwarmMember::SwarmMember(int id, const Position& pos) {
    m_id = id;
    m_pos = pos;
}

int SwarmMember::getId() const {
    return m_id;
}

Position SwarmMember::getPos() const {
    return m_pos;
}

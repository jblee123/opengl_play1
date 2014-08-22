#include <algorithm>

#include "Swarm.h"

Swarm::Swarm() :
    m_leader(nullptr) {
}

Swarm::~Swarm() {
}

bool Swarm::contains(SwarmMember* member) const {
    for (auto m : m_members) {
        if (m == member) {
            return true;
        }
    }
    return false;
}

void Swarm::addMember(SwarmMember* member) {
    m_members.push_back(member);
    if (!m_leader) {
        m_leader = member;
    }
}

void Swarm::removeMember(SwarmMember* member) {
    auto result = std::find(m_members.begin(), m_members.end(), member);
    if (result != m_members.end()) {
        m_members.erase(result);
        if (m_leader == member) {
            m_leader = m_members.empty() ? nullptr : m_members[0];
        }
    }
}

void Swarm::setLeader(SwarmMember* member) {
    if (contains(member)) {
        m_leader = member;
    }
}

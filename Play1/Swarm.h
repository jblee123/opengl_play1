#pragma once

#include <vector>

#include "SwarmMember.h"

class Swarm
{
public:
    Swarm();
    ~Swarm();

    bool contains(SwarmMember* member) const;
    void addMember(SwarmMember* member);
    void removeMember(SwarmMember* member);
    void setLeader(SwarmMember* member);

protected:
    std::vector<SwarmMember*> m_members;
    SwarmMember* m_leader;
};

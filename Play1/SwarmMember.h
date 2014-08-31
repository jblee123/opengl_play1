#pragma once

#include <windows.h>

#include "Color.h"
#include "Position.h"

class SwarmMember {
public:
    SwarmMember(int id);
    SwarmMember(int id, const Position& pos);

    int getId() const;
    Position getPos() const;
    Position getPosForAnimation(DWORD time) const;

    Color getColor() const;
    void setColor(Color color);

protected:
    int m_id;
    Position m_pos;
    Color m_color;
};

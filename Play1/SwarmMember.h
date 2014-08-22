#include "Position.h"

class SwarmMember {
public:
    SwarmMember(int id);
    SwarmMember(int id, const Position& pos);

    int getId() const;
    Position getPos() const;

protected:
    int m_id;
    Position m_pos;
};

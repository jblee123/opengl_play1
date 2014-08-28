#include "Position.h"

#include "Color.h"

class SwarmMember {
public:
    SwarmMember(int id);
    SwarmMember(int id, const Position& pos);

    int getId() const;
    Position getPos() const;

    Color getColor() const;
    void setColor(Color color);

protected:
    int m_id;
    Position m_pos;
    Color m_color;
};

#pragma once

#include "BufferDrawable.h"
#include "Matrix4Df.h"
#include "SwarmMember.h"

class DrawnSwarm :
    public BufferDrawable
{
public:
    DrawnSwarm(int width, int height, int swarm_size);
    ~DrawnSwarm();

    virtual void setup();
    virtual void draw(const mat4df::Mat4Df& modelView, const mat4df::Mat4Df& projection);

protected:
    void createSwarm();

    GLuint m_posOffset;
    std::vector<SwarmMember*> m_swarm;
    int m_width;
    int m_height;
    int m_swarm_size;
};

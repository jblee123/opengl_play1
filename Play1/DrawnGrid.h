#pragma once

#include "BufferDrawable.h"
#include "Matrix4Df.h"

class DrawnGrid :
    public BufferDrawable
{
public:
    DrawnGrid(int width, int height);
    virtual ~DrawnGrid();

    virtual void setup();
    virtual void draw(const mat4df::Mat4Df& modelView, const mat4df::Mat4Df& projection);

protected:
    int m_width;
    int m_height;
    GLuint m_pointCount;
};

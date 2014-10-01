#pragma once

#include "BufferDrawable.h"
#include "Camera.h"

class AxisDisplay :
    public BufferDrawable
{
public:
    AxisDisplay();
    virtual ~AxisDisplay();

    virtual void setup();
    virtual void draw(int width, int height, const Camera& camera);

protected:
    mat4df::Mat4Df m_perspectiveMatrix;
};

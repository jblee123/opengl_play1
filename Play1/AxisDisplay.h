#pragma once

#include "BufferDrawable.h"

class AxisDisplay :
    public BufferDrawable
{
public:
    AxisDisplay();
    virtual ~AxisDisplay();

    virtual void setup();
    virtual void draw(int width, int height);
};

#pragma once

#include <vector>

#include <GL/glew.h>

class BufferDrawable
{
public:
    BufferDrawable();
    virtual ~BufferDrawable();

    virtual void setProgram(GLuint program);
    
    virtual void setup();
    virtual void cleanup();

protected:
    void pushCoord(int x, int y, int z, std::vector<GLfloat>& coords);
    void pushCoord(int x, int y, std::vector<GLfloat>& coords);

    GLuint m_program;
    GLuint m_vao;
    GLuint m_vbo;
};

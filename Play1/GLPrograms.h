#pragma once

#include <windows.h>
#include <GL/gl.h>

class GLPrograms
{
public:
    GLPrograms();
    ~GLPrograms();

    void compilePrograms();
    void cleanupPrograms();

    GLuint getProg1() const;

protected:
    void compileProgram1();

    GLuint m_prog1;
};

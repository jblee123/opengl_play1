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
    GLuint getProg2() const;

protected:
    void compileProgram(
        const GLchar* vertexShaderSource,
        const GLchar* fragmentShaderSource,
        GLuint& prog);

    void compileProgram1();
    void compileProgram2();

    GLuint m_prog1;
    GLuint m_prog2;
};

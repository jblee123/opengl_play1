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

    GLuint getSimpleProg() const;
    GLuint getProg1() const;
    GLuint getProg2() const;
    GLuint getProg3() const;

protected:
    void compileProgram(
        const GLchar* vertexShaderSource,
        const GLchar* fragmentShaderSource,
        GLuint& prog);

    void compileSimpleProgram();
    void compileProgram1();
    void compileProgram2();
    void compileProgram3();

    static void cleanupProgram(GLuint& prog);

    GLuint m_simpleProg;
    GLuint m_prog1;
    GLuint m_prog2;
    GLuint m_prog3;
};

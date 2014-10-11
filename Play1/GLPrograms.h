#pragma once

#include <GL/gl.h>

class GLPrograms
{
public:
    GLPrograms();
    ~GLPrograms();

    void compilePrograms();
    void cleanupPrograms();

    GLuint getSimpleProg() const;
    GLuint get2dProg() const;
    GLuint getSimpleTextureProg() const;
    GLuint getProg1() const;
    GLuint getProg2() const;
    GLuint getProg3() const;

protected:
    GLint compileShader(GLuint shaderType, const GLchar* shaderSource);
    GLuint compileProgram(
        const GLchar* vertexShaderSource,
        const GLchar* fragmentShaderSource);

    void compileSimpleProgram();
    void compile2dProgram();
    void compileSimpleTextureProgram();
    void compileProgram1();
    void compileProgram2();
    void compileProgram3();

    GLuint m_simpleProg;
    GLuint m_2dProg;
    GLuint m_simpleTextureProg;
    GLuint m_prog1;
    GLuint m_prog2;
    GLuint m_prog3;
};

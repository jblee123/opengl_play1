#include <GL/glew.h>

#include "GLPrograms.h"

GLPrograms::GLPrograms() :
    m_prog1(0) {
}

GLPrograms::~GLPrograms() {
}

void GLPrograms::compilePrograms() {
    compileProgram1();
}

void GLPrograms::cleanupPrograms() {
    if (m_prog1) {
        glDeleteProgram(m_prog1);
        m_prog1 = 0;
    }
}

GLuint GLPrograms::getProg1() const {
    return m_prog1;
}

void GLPrograms::compileProgram1() {
    const GLchar* VERTEX_SHADER_SOURCE[] = {
        "#version 410 core                                  \n"
        "                                                   \n"
        "layout (location = 0) in vec4 offset;              \n"
        "layout (location = 1) in vec4 color;               \n"
        "                                                   \n"
        "out vec4 vs_color;                                 \n"
        "                                                   \n"
        "void main(void) {                                  \n"
        "    const vec4 vertices[] = vec4[3](               \n"
        "        vec4( 0.25, -0.25, 0.5, 1.0),              \n"
        "        vec4(-0.25, -0.25, 0.5, 1.0),              \n"
        "        vec4( 0.25,  0.25, 0.5, 1.0));             \n"
        "    gl_Position = vertices[gl_VertexID] + offset;  \n"
        "    vs_color = color;                              \n"
        "}                                                  \n"
    };

    const GLchar* FRAGMENT_SHADER_SOURCE[] = {
        "#version 410 core      \n"
        "                       \n"
        "in vec4 vs_color;      \n"
        "out vec4 color;        \n"
        "void main(void) {      \n"
        "    color = vs_color;  \n"
        "}                      \n"
    };

    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, VERTEX_SHADER_SOURCE, nullptr);
    glCompileShader(vertexShader);

    // fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, FRAGMENT_SHADER_SOURCE, nullptr);
    glCompileShader(fragmentShader);

    // Create program, attach shaders to it, and link it
    m_prog1 = glCreateProgram();
    glAttachShader(m_prog1, vertexShader);
    glAttachShader(m_prog1, fragmentShader);
    glLinkProgram(m_prog1);

    // Delete the shaders as the program has them now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

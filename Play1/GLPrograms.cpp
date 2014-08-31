#include <GL/glew.h>

#include "GLPrograms.h"

GLPrograms::GLPrograms() :
    m_prog1(0),
    m_prog2(0) {
}

GLPrograms::~GLPrograms() {
}

void GLPrograms::compilePrograms() {
    compileProgram1();
    compileProgram2();
}

void GLPrograms::cleanupPrograms() {
    if (m_prog1) {
        glDeleteProgram(m_prog1);
        m_prog1 = 0;
    }
    if (m_prog2) {
        glDeleteProgram(m_prog2);
        m_prog2 = 0;
    }
}

GLuint GLPrograms::getProg1() const {
    return m_prog1;
}

GLuint GLPrograms::getProg2() const {
    return m_prog2;
}

void GLPrograms::compileProgram(
    const GLchar* vertexShaderSource,
    const GLchar* fragmentShaderSource,
    GLuint& prog) {

    const GLchar* VERTEX_SHADER_SOURCE[] = { vertexShaderSource };
    const GLchar* FRAGMENT_SHADER_SOURCE[] = { fragmentShaderSource };

    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, VERTEX_SHADER_SOURCE, nullptr);
    glCompileShader(vertexShader);

    // fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, FRAGMENT_SHADER_SOURCE, nullptr);
    glCompileShader(fragmentShader);

    // Create program, attach shaders to it, and link it
    prog = glCreateProgram();
    glAttachShader(prog, vertexShader);
    glAttachShader(prog, fragmentShader);
    glLinkProgram(prog);

    // Delete the shaders as the program has them now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void GLPrograms::compileProgram1() {
    const GLchar* VERTEX_SHADER_SOURCE =
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
        "}                                                  \n";

    const GLchar* FRAGMENT_SHADER_SOURCE =
        "#version 410 core      \n"
        "                       \n"
        "in vec4 vs_color;      \n"
        "out vec4 color;        \n"
        "void main(void) {      \n"
        "    color = vs_color;  \n"
        "}                      \n";

    compileProgram(
        VERTEX_SHADER_SOURCE,
        FRAGMENT_SHADER_SOURCE,
        m_prog1);
}

void GLPrograms::compileProgram2() {
    const GLchar* VERTEX_SHADER_SOURCE =
        "#version 410 core                          \n"
        "                                           \n"
        "layout (location = 0) in vec4 vertex_pos;  \n"
        "layout (location = 1) in vec4 inst_color;  \n"
        "layout (location = 2) in vec4 inst_pos;    \n"
        "layout (location = 3) in float inst_angle; \n"
        "                                           \n"
        "out vec4 vs_color;                         \n"
        "                                           \n"
        "void main(void) {                          \n"
        "    mat4 trans = mat4(1.0);                \n"
        "    trans[0][0] = cos(inst_angle);         \n"
        "    trans[1][1] = trans[0][0];             \n"
        "    trans[0][1] = sin(inst_angle);         \n"
        "    trans[1][0] = -trans[0][1];            \n"
        "    trans[3][0] = inst_pos.x;              \n"
        "    trans[3][1] = inst_pos.y;              \n"
        "    gl_Position = trans * vertex_pos;      \n"
        "    vs_color = inst_color;                 \n"
        "}                                          \n";

    const GLchar* FRAGMENT_SHADER_SOURCE =
        "#version 410 core      \n"
        "                       \n"
        "in vec4 vs_color;      \n"
        "out vec4 color;        \n"
        "void main(void) {      \n"
        "    color = vs_color;  \n"
        "}                      \n";

    compileProgram(
        VERTEX_SHADER_SOURCE,
        FRAGMENT_SHADER_SOURCE,
        m_prog2);
}

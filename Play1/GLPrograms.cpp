#include <cstdlib>
#include <string>

#include <GL/glew.h>

#include "GLPrograms.h"

GLPrograms::GLPrograms() :
    m_simpleProg(0),
    m_2dProg(0),
    m_prog1(0),
    m_prog2(0),
    m_prog3(0) {
}

GLPrograms::~GLPrograms() {
}

void GLPrograms::compilePrograms() {
    compileSimpleProgram();
    compile2dProgram();
    compileProgram1();
    compileProgram2();
    compileProgram3();
}

void GLPrograms::cleanupPrograms() {
    auto cleanupProgram = [](GLuint& prog) {
        if (prog) {
            glDeleteProgram(prog);
            prog = 0;
        }
    };

    cleanupProgram(m_simpleProg);
    cleanupProgram(m_2dProg);
    cleanupProgram(m_prog1);
    cleanupProgram(m_prog2);
    cleanupProgram(m_prog3);
}

GLuint GLPrograms::getSimpleProg() const {
    return m_simpleProg;
}

GLuint GLPrograms::get2dProg() const {
    return m_2dProg;
}

GLuint GLPrograms::getProg1() const {
    return m_prog1;
}

GLuint GLPrograms::getProg2() const {
    return m_prog2;
}

GLuint GLPrograms::getProg3() const {
    return m_prog3;
}

GLint GLPrograms::compileShader(GLuint shaderType, const GLchar* shaderSource) {
    const GLchar* SHADER_SOURCE[] = { shaderSource };

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, SHADER_SOURCE, nullptr);
    glCompileShader(shader);

    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus) {
        printf("%s shader failed to compile",
            (shaderType == GL_VERTEX_SHADER) ? "vertex" : "fragment");

        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        GLchar* infoLog = new GLchar[logLength + 1];
        glGetShaderInfoLog(shader, logLength, nullptr, infoLog);
        printf("log info:\n%s", infoLog);
        delete[] infoLog;
    }

    return shader;
}

GLuint GLPrograms::compileProgram(
    const GLchar* vertexShaderSource,
    const GLchar* fragmentShaderSource) {

    const GLchar* VERTEX_SHADER_SOURCE[] = { vertexShaderSource };
    const GLchar* FRAGMENT_SHADER_SOURCE[] = { fragmentShaderSource };

    // vertex shader
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    
    // Create program, attach shaders to it, and link it
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vertexShader);
    glAttachShader(prog, fragmentShader);
    glLinkProgram(prog);

    // Delete the shaders as the program has them now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return prog;
}

void GLPrograms::compileSimpleProgram() {
    const GLchar* VERTEX_SHADER_SOURCE =
        "#version 410 core                                      \n"
        "#extension GL_ARB_explicit_uniform_location : require  \n"
        "                                                       \n"
        "layout (location = 0) in vec4 vertex_pos;              \n"
        "                                                       \n"
        "layout (location = 0) uniform mat4 mv_matrix;          \n"
        "layout (location = 1) uniform mat4 proj_matrix;        \n"
        "layout (location = 2) uniform vec4 color;              \n"
        "                                                       \n"
        "out vec4 vs_color;                                     \n"
        "                                                       \n"
        "void main(void) {                                      \n"
        "    gl_Position =                                      \n"
        "        proj_matrix *                                  \n"
        "        mv_matrix *                                    \n"
        "        vertex_pos;                                    \n"
        "    vs_color = color;                                  \n"
        "}                                                      \n";

    const GLchar* FRAGMENT_SHADER_SOURCE =
        "#version 410 core      \n"
        "                       \n"
        "in vec4 vs_color;      \n"
        "out vec4 color;        \n"
        "void main(void) {      \n"
        "    color = vs_color;  \n"
        "}                      \n";

    m_simpleProg = compileProgram(
        VERTEX_SHADER_SOURCE,
        FRAGMENT_SHADER_SOURCE);
}

void GLPrograms::compile2dProgram() {
    const GLchar* VERTEX_SHADER_SOURCE =
        "#version 410 core                                      \n"
        "#extension GL_ARB_explicit_uniform_location : require  \n"
        "                                                       \n"
        "layout (location = 0) in vec2 vertex_pos;              \n"
        "                                                       \n"
        "layout (location = 0) uniform float width;             \n"
        "layout (location = 1) uniform float height;            \n"
        "layout (location = 2) uniform vec4 color;              \n"
        "                                                       \n"
        "out vec4 vs_color;                                     \n"
        "                                                       \n"
        "void main(void) {                                      \n"
        "    gl_Position = vec4(                                \n"
        "        (2 * vertex_pos[0] / width) - 1,               \n"
        "        1 - (2 * vertex_pos[1] / height),              \n"
        "        0, 1);                                         \n"
        "    vs_color = color;                                  \n"
        "}                                                      \n";

    const GLchar* FRAGMENT_SHADER_SOURCE =
        "#version 410 core      \n"
        "                       \n"
        "in vec4 vs_color;      \n"
        "out vec4 color;        \n"
        "void main(void) {      \n"
        "    color = vs_color;  \n"
        "}                      \n";

    m_2dProg = compileProgram(
        VERTEX_SHADER_SOURCE,
        FRAGMENT_SHADER_SOURCE);
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

    m_prog1 = compileProgram(
        VERTEX_SHADER_SOURCE,
        FRAGMENT_SHADER_SOURCE);
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

    m_prog2 = compileProgram(
        VERTEX_SHADER_SOURCE,
        FRAGMENT_SHADER_SOURCE);
}

void GLPrograms::compileProgram3() {
    const GLchar* VERTEX_SHADER_SOURCE =
        "#version 410 core                                     \n"
        "#extension GL_ARB_explicit_uniform_location : require \n"
        "                                                      \n"
        "layout (location = 0) in vec4 vertex_pos;             \n"
        "layout (location = 1) in vec4 inst_color;             \n"
        "layout (location = 2) in vec4 inst_pos;               \n"
        "layout (location = 3) in float inst_angle;            \n"
        "                                                      \n"
        "layout (location = 0) uniform mat4 mv_matrix;         \n"
        "layout (location = 1) uniform mat4 proj_matrix;       \n"
        "                                                      \n"
        "out vec4 vs_color;                                    \n"
        "                                                      \n"
        "void main(void) {                                     \n"
        "    mat4 trans = mat4(1.0);                           \n"
        "    trans[0][0] = cos(inst_angle);                    \n"
        "    trans[1][1] = trans[0][0];                        \n"
        "    trans[0][1] = sin(inst_angle);                    \n"
        "    trans[1][0] = -trans[0][1];                       \n"
        "    trans[3][0] = inst_pos.x;                         \n"
        "    trans[3][1] = inst_pos.y;                         \n"
        "    gl_Position =                                     \n"
        "        proj_matrix *                                 \n"
        "        mv_matrix *                                   \n"
        "        trans *                                       \n"
        "        vertex_pos;                                   \n"
        "    vs_color = inst_color;                            \n"
        "}                                                     \n";

    const GLchar* FRAGMENT_SHADER_SOURCE =
        "#version 410 core      \n"
        "                       \n"
        "in vec4 vs_color;      \n"
        "out vec4 color;        \n"
        "void main(void) {      \n"
        "    color = vs_color;  \n"
        "}                      \n";

    m_prog3 = compileProgram(
        VERTEX_SHADER_SOURCE,
        FRAGMENT_SHADER_SOURCE);
}

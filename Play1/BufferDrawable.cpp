#include "BufferDrawable.h"


BufferDrawable::BufferDrawable() :
    m_program(0),
    m_vao(0),
    m_vbo(0)
{
}

BufferDrawable::~BufferDrawable()
{
}

void BufferDrawable::setProgram(GLuint program) {
    m_program = program;
}

void BufferDrawable::setup() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
}

void BufferDrawable::cleanup() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void BufferDrawable::pushCoord(int x, int y, int z, std::vector<GLfloat>& coords) {
    coords.push_back((GLfloat)x);
    coords.push_back((GLfloat)y);
    coords.push_back((GLfloat)0);
    coords.push_back((GLfloat)1);
}

void BufferDrawable::pushCoord(int x, int y, std::vector<GLfloat>& coords) {
    coords.push_back((GLfloat)x);
    coords.push_back((GLfloat)y);
    coords.push_back((GLfloat)0);
    coords.push_back((GLfloat)1);
}

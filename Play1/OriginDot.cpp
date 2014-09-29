#include "OriginDot.h"

OriginDot::OriginDot() :
    BufferDrawable(),
    m_pointCount(0)
{
}

OriginDot::~OriginDot()
{
}

void OriginDot::setup() {

    BufferDrawable::setup();

    std::vector<GLfloat> dotCoords;

    const int DOT_RADIUS = 10;
    pushCoord(-DOT_RADIUS, 0, dotCoords);
    pushCoord(0, DOT_RADIUS * 2, dotCoords);
    pushCoord(DOT_RADIUS, 0, dotCoords);
    pushCoord(0, -DOT_RADIUS, dotCoords);
    pushCoord(-DOT_RADIUS, 0, dotCoords);

    m_pointCount = 5;

    const GLuint DOT_BUFFER_SIZE = sizeof(GLfloat) * dotCoords.size();

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, DOT_BUFFER_SIZE, dotCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void OriginDot::draw(const mat4df::Mat4Df& modelView, const mat4df::Mat4Df& projection) {
    glUseProgram(m_program);

    const GLuint PROG4_MODEL_VIEW_LOC = 0;
    const GLuint PROG4_PROJ_LOC = 1;
    const GLuint PROG4_COLOR_LOC = 2;
    glUniformMatrix4fv(PROG4_MODEL_VIEW_LOC, 1, GL_FALSE, modelView.getBuf());
    glUniformMatrix4fv(PROG4_PROJ_LOC, 1, GL_FALSE, projection.getBuf());
    glUniform4f(PROG4_COLOR_LOC, 1, 0, 0, 1);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINE_STRIP, 0, m_pointCount);
}

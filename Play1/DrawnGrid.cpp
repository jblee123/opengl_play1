#include <algorithm>

#include "DrawnGrid.h"

DrawnGrid::DrawnGrid(int width, int height) :
    BufferDrawable(),
    m_width(width),
    m_height(height),
    m_pointCount(0)
{
}

DrawnGrid::~DrawnGrid()
{
}

void DrawnGrid::setup() {

    BufferDrawable::setup();

    const int GRID_SIZE = 100;
    std::vector<GLfloat> outlineCoords;

    int horizontalLineCount = (m_height / GRID_SIZE) + 1;
    for (int i = 0; i < horizontalLineCount; i++) {
        float y = (float)std::min(i * GRID_SIZE, m_height);
        pushCoord4d(0, y, outlineCoords);
        pushCoord4d((float)m_width, y, outlineCoords);
    }

    int verticalLineCount = (m_width / GRID_SIZE) + 1;
    for (int i = 0; i < verticalLineCount; i++) {
        float x = (float)std::min(i * GRID_SIZE, m_width);
        pushCoord4d(x, 0, outlineCoords);
        pushCoord4d(x, (float)m_height, outlineCoords);
    }

    m_pointCount = (horizontalLineCount + verticalLineCount) * 2;

    const GLuint OUTLINE_BUFFER_SIZE = sizeof(GLfloat) * outlineCoords.size();

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, OUTLINE_BUFFER_SIZE, outlineCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void DrawnGrid::draw(const mat4df::Mat4Df& modelView, const mat4df::Mat4Df& projection) {
    glUseProgram(m_program);

    const GLuint PROG4_MODEL_VIEW_LOC = 0;
    const GLuint PROG4_PROJ_LOC = 1;
    const GLuint PROG4_COLOR_LOC = 2;
    glUniformMatrix4fv(PROG4_MODEL_VIEW_LOC, 1, GL_FALSE, modelView.getBuf());
    glUniformMatrix4fv(PROG4_PROJ_LOC, 1, GL_FALSE, projection.getBuf());
    glUniform4f(PROG4_COLOR_LOC, 1, 1, 0, 1);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINES, 0, m_pointCount);
}

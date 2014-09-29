#include "AxisDisplay.h"

AxisDisplay::AxisDisplay()
{
}

AxisDisplay::~AxisDisplay()
{
}

void AxisDisplay::setup() {

    BufferDrawable::setup();

    // will need a max of 6 points
    const GLuint BUFFER_SIZE = sizeof(GLfloat) * 2 * 6;

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, nullptr, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void AxisDisplay::draw(int width, int height) {

    const float DISPLAY_WIDTH = 100;
    const float DISPLAY_HEIGHT = DISPLAY_WIDTH;
    const float DISPLAY_MARGIN = 20;

    glUseProgram(m_program);

    const GLuint WIDTH_LOC = 0;
    const GLuint HEIGHT_LOC = 1;
    const GLuint COLOR_LOC = 2;
    glUniform1f(WIDTH_LOC, (float)width);
    glUniform1f(HEIGHT_LOC, (float)height);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    const int COORD_ELEM_COUNT = 2 * 6;
    GLfloat coords[COORD_ELEM_COUNT];

    glUniform4f(COLOR_LOC, 0, 0, 0, 1);

    coords[0] = width - DISPLAY_WIDTH - DISPLAY_MARGIN;
    coords[1] = height - DISPLAY_HEIGHT - DISPLAY_MARGIN;
    coords[2] = coords[0] + DISPLAY_WIDTH;
    coords[3] = coords[1];
    coords[4] = coords[0];
    coords[5] = coords[1] + DISPLAY_HEIGHT;
    coords[6] = coords[0] + DISPLAY_WIDTH;
    coords[7] = coords[1] + DISPLAY_HEIGHT;
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUniform4f(COLOR_LOC, 1, 1, 1, 1);

    coords[0] = width - DISPLAY_WIDTH - DISPLAY_MARGIN;
    coords[1] = height - DISPLAY_HEIGHT - DISPLAY_MARGIN;
    coords[2] = coords[0] + DISPLAY_WIDTH;
    coords[3] = coords[1];
    coords[4] = coords[0] + DISPLAY_WIDTH;
    coords[5] = coords[1] + DISPLAY_HEIGHT;
    coords[6] = coords[0];
    coords[7] = coords[1] + DISPLAY_HEIGHT;
    coords[8] = coords[0];
    coords[9] = coords[1];
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);
    glDrawArrays(GL_LINE_STRIP, 0, 5);


}

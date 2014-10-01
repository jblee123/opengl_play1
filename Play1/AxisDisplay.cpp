#include "AxisDisplay.h"
#include "projection.h"

AxisDisplay::AxisDisplay()
{
    const float FOV = 100;
    const float WIDTH = 2;
    const float HEIGHT = 2;
    const float NEAR_DIST = 0.5;
    const float FAR_DIST = 3;
    m_perspectiveMatrix = projection::createPerspective(FOV, WIDTH, HEIGHT, NEAR_DIST, FAR_DIST);
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

void AxisDisplay::draw(int width, int height, const Camera& camera) {

    const float DISPLAY_WIDTH = 100;
    const float DISPLAY_HEIGHT = DISPLAY_WIDTH;
    const float DISPLAY_MARGIN = 20;

    int i;

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

    i = 0;
    coords[i++] = width - DISPLAY_WIDTH - DISPLAY_MARGIN;
    coords[i++] = height - DISPLAY_HEIGHT - DISPLAY_MARGIN;
    coords[i++] = coords[0] + DISPLAY_WIDTH;
    coords[i++] = coords[1];
    coords[i++] = coords[0];
    coords[i++] = coords[1] + DISPLAY_HEIGHT;
    coords[i++] = coords[0] + DISPLAY_WIDTH;
    coords[i++] = coords[1] + DISPLAY_HEIGHT;
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUniform4f(COLOR_LOC, 1, 1, 1, 1);

    i = 0;
    coords[i++] = width - DISPLAY_WIDTH - DISPLAY_MARGIN;
    coords[i++] = height - DISPLAY_HEIGHT - DISPLAY_MARGIN;
    coords[i++] = coords[0] + DISPLAY_WIDTH;
    coords[i++] = coords[1];
    coords[i++] = coords[0] + DISPLAY_WIDTH;
    coords[i++] = coords[1] + DISPLAY_HEIGHT;
    coords[i++] = coords[0];
    coords[i++] = coords[1] + DISPLAY_HEIGHT;
    coords[i++] = coords[0];
    coords[i++] = coords[1];
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);
    glDrawArrays(GL_LINE_STRIP, 0, 5);

    const float AXIS_CENTER_X = width - (DISPLAY_WIDTH / 2) - DISPLAY_MARGIN;
    const float AXIS_CENTER_Y = height - (DISPLAY_HEIGHT / 2) - DISPLAY_MARGIN;

    const float SCALE_FACTOR = DISPLAY_WIDTH / 2;

    vec3df::Vec3Df fwd = camera.getFwd() * SCALE_FACTOR;
    vec3df::Vec3Df up = camera.getUp() * -SCALE_FACTOR;
    vec3df::Vec3Df side = camera.getSide() * SCALE_FACTOR;

    fwd = mat4df::mul(m_perspectiveMatrix, fwd);
    up = mat4df::mul(m_perspectiveMatrix, up);
    side = mat4df::mul(m_perspectiveMatrix, side);

    fwd = vec3df::translate(fwd, AXIS_CENTER_X, AXIS_CENTER_Y, 0);
    up = vec3df::translate(up, AXIS_CENTER_X, AXIS_CENTER_Y, 0);
    side = vec3df::translate(side, AXIS_CENTER_X, AXIS_CENTER_Y, 0);

    i = 0;
    coords[i++] = AXIS_CENTER_X;
    coords[i++] = AXIS_CENTER_Y;
    coords[i++] = fwd(0);
    coords[i++] = fwd(1);
    coords[i++] = AXIS_CENTER_X;
    coords[i++] = AXIS_CENTER_Y;
    coords[i++] = up(0);
    coords[i++] = up(1);
    coords[i++] = AXIS_CENTER_X;
    coords[i++] = AXIS_CENTER_Y;
    coords[i++] = side(0);
    coords[i++] = side(1);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);

    glUniform4f(COLOR_LOC, 1, 0, 0, 1);
    glDrawArrays(GL_LINES, 0, 2);

    glUniform4f(COLOR_LOC, 0, 1, 0, 1);
    glDrawArrays(GL_LINES, 2, 2);

    glUniform4f(COLOR_LOC, 0, 0, 1, 1);
    glDrawArrays(GL_LINES, 4, 2);
}

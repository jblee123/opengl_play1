#include "DrawnSwarm.h"

#include "Color.h"
#include "Utils.h"

DrawnSwarm::DrawnSwarm(int width, int height, int swarm_size) :
    BufferDrawable(),
    m_posOffset(0),
    m_width(width),
    m_height(height),
    m_swarm_size(swarm_size)
{
}

DrawnSwarm::~DrawnSwarm()
{
}

void DrawnSwarm::setup() {

    BufferDrawable::setup();

    createSwarm();

    GLfloat memberCoords[] = {
        -10, 10, 0, 1,
        0, -25, 0, 1,
        0, 0, 0, 1,
        10, 10, 0, 1
    };

    const GLuint MEMBER_COORDS_OFFSET = 0;
    const GLuint MEMBER_COORDS_SIZE = sizeof(memberCoords);
    const GLuint MEMBER_COLOR_OFFSET = MEMBER_COORDS_OFFSET + MEMBER_COORDS_SIZE;
    const GLuint MEMBER_COLOR_SIZE = sizeof(GLfloat) * 4 * m_swarm.size();
    const GLuint MEMBER_POS_OFFSET = MEMBER_COLOR_OFFSET + MEMBER_COLOR_SIZE;
    const GLuint MEMBER_POS_SIZE = sizeof(GLfloat) * 5 * m_swarm.size();
    const GLuint SWARM_BUFFER_SIZE = MEMBER_COORDS_SIZE + MEMBER_COLOR_SIZE + MEMBER_POS_SIZE;

    m_posOffset = MEMBER_POS_OFFSET;

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, SWARM_BUFFER_SIZE, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, MEMBER_COORDS_OFFSET, MEMBER_COORDS_SIZE, memberCoords);

    float* colorPtr = (float*)((char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY) + MEMBER_COLOR_OFFSET);
    for (auto member : m_swarm) {
        Color color = member->getColor();
        *colorPtr++ = (float)color.r / 255.0f;
        *colorPtr++ = (float)color.g / 255.0f;
        *colorPtr++ = (float)color.b / 255.0f;
        *colorPtr++ = (float)color.a / 255.0f;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)MEMBER_COORDS_OFFSET);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)MEMBER_COLOR_OFFSET);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)MEMBER_POS_OFFSET);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(MEMBER_POS_OFFSET + (sizeof(GLfloat) * 4)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
}

void DrawnSwarm::draw(const mat4df::Mat4Df& modelView, const mat4df::Mat4Df& projection) {
    glUseProgram(m_program);

    const GLuint PROG3_MODEL_VIEW_LOC = 0;
    const GLuint PROG3_PROJ_LOC = 1;
    glUniformMatrix4fv(PROG3_MODEL_VIEW_LOC, 1, GL_FALSE, modelView.getBuf());
    glUniformMatrix4fv(PROG3_PROJ_LOC, 1, GL_FALSE, projection.getBuf());

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    float* posPtr = (float*)((char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY) + m_posOffset);
    for (auto member : m_swarm) {
        Position pos = member->getPosForAnimation(timeGetTime());
        //Position pos = member->getPos();
        vec3df::Vec3Df loc = pos.getLocation();
        *posPtr++ = loc(0);
        *posPtr++ = loc(1);
        *posPtr++ = loc(2);
        *posPtr++ = 0.0f;
        *posPtr++ = pos.getHeading();
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_swarm_size);
}

void DrawnSwarm::createSwarm() {
    for (int i = 0; i < m_swarm_size; i++) {
        SwarmMember* member = (i == 0) ?
            new SwarmMember(
                i + 1, 
                Position(
                    vec3df::create(
                        m_width / 2.0f,
                        m_height / 2.0f,
                        -1.0f),
                    0)) :
            new SwarmMember(
                i + 1,
                Position(
                    vec3df::create(
                        randf() * m_width,
                        randf() * m_height,
                        -1.0f),
                    randf() * (2 * (float)PI)));
        //g_swarm.addMember(member);
        Color color;
        color.r = rand() % 256;
        color.g = rand() % 256;
        color.b = rand() % 256;
        color.a = 255;
        member->setColor(color);
        m_swarm.push_back(member);
    }
}

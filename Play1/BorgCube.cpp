#include "BorgCube.h"

#include <Windows.h>
#include <Gdiplus.h>
#include <direct.h>

BorgCube::BorgCube()
{
}

BorgCube::~BorgCube()
{
}

void BorgCube::setup() {
    BufferDrawable::setup();

    Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(L"images/borg.jpg");

    Gdiplus::BitmapData bitmapData;
    Gdiplus::Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
    bitmap->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

    unsigned char* data = (unsigned char*)bitmapData.Scan0;

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexStorage2D(
        GL_TEXTURE_2D, 1, GL_RGBA8,
        bitmap->GetWidth(), bitmap->GetHeight());
    glTexSubImage2D(
        GL_TEXTURE_2D, 0,
        0, 0, bitmap->GetWidth(), bitmap->GetHeight(),
        GL_RGBA, GL_UNSIGNED_BYTE, data);

    bitmap->UnlockBits(&bitmapData);
    delete bitmap;






    const unsigned int COORDS_PER_FACE = 4;
    const unsigned int VERTEX_COMPONENTS_PER_COORD = 3;
    const unsigned int TEXTURE_COMPONENTS_PER_COORD = 2;

    const GLuint VERTEX_COORDS_OFFSET = 0;
    const GLuint VERTEX_COORDS_SIZE = sizeof(GLfloat) * COORDS_PER_FACE * VERTEX_COMPONENTS_PER_COORD;
    const GLuint TEXTURE_COORDS_OFFSET = VERTEX_COORDS_OFFSET + VERTEX_COORDS_SIZE;
    const GLuint TEXTURE_COORDS_SIZE = sizeof(GLfloat) * COORDS_PER_FACE * TEXTURE_COMPONENTS_PER_COORD;

    const unsigned int BUFFER_SIZE = VERTEX_COORDS_SIZE + TEXTURE_COORDS_SIZE;

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, NULL, GL_STATIC_DRAW);

    float* textureCoordPtr = (float*)((char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY) + TEXTURE_COORDS_OFFSET);
    *textureCoordPtr++ = 0;
    *textureCoordPtr++ = 0;
    *textureCoordPtr++ = 1;
    *textureCoordPtr++ = 0;
    *textureCoordPtr++ = 0;
    *textureCoordPtr++ = 1;
    *textureCoordPtr++ = 1;
    *textureCoordPtr++ = 1;
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glVertexAttribPointer(0, VERTEX_COMPONENTS_PER_COORD, GL_FLOAT, GL_FALSE, 0, (GLvoid*)VERTEX_COORDS_OFFSET);
    glVertexAttribPointer(1, TEXTURE_COMPONENTS_PER_COORD, GL_FLOAT, GL_FALSE, 0, (GLvoid*)TEXTURE_COORDS_OFFSET);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void BorgCube::draw(const mat4df::Mat4Df& modelView, const mat4df::Mat4Df& projection) {
    glUseProgram(m_program);

    //glDepthFunc(GL_LESS);
    //glEnable(GL_DEPTH_TEST);
 
    const GLuint PROG_MODEL_VIEW_LOC = 0;
    const GLuint PROG_PROJ_LOC = 1;
    glUniformMatrix4fv(PROG_MODEL_VIEW_LOC, 1, GL_FALSE, modelView.getBuf());
    glUniformMatrix4fv(PROG_PROJ_LOC, 1, GL_FALSE, projection.getBuf());

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    const GLfloat BOTTOM_OFFSET = 300;
    const GLfloat LEFT_OFFSET = 1000;
    const GLfloat SIDE_LEN = 300;
    const GLfloat Z_OFFSET = SIDE_LEN / 2;

    std::vector<GLfloat> coords;

    // front
    pushCoord3d(LEFT_OFFSET,            BOTTOM_OFFSET + SIDE_LEN, Z_OFFSET, coords);
    pushCoord3d(LEFT_OFFSET + SIDE_LEN, BOTTOM_OFFSET + SIDE_LEN, Z_OFFSET, coords);
    pushCoord3d(LEFT_OFFSET,            BOTTOM_OFFSET,            Z_OFFSET, coords);
    pushCoord3d(LEFT_OFFSET + SIDE_LEN, BOTTOM_OFFSET,            Z_OFFSET, coords);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * coords.size(), coords.data());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // right
    coords.clear();
    pushCoord3d(LEFT_OFFSET + SIDE_LEN, BOTTOM_OFFSET + SIDE_LEN, Z_OFFSET,            coords);
    pushCoord3d(LEFT_OFFSET + SIDE_LEN, BOTTOM_OFFSET + SIDE_LEN, Z_OFFSET - SIDE_LEN, coords);
    pushCoord3d(LEFT_OFFSET + SIDE_LEN, BOTTOM_OFFSET,            Z_OFFSET,            coords);
    pushCoord3d(LEFT_OFFSET + SIDE_LEN, BOTTOM_OFFSET,            Z_OFFSET - SIDE_LEN, coords);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * coords.size(), coords.data());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // back
    coords.clear();
    pushCoord3d(LEFT_OFFSET + SIDE_LEN, BOTTOM_OFFSET + SIDE_LEN, Z_OFFSET - SIDE_LEN, coords);
    pushCoord3d(LEFT_OFFSET,            BOTTOM_OFFSET + SIDE_LEN, Z_OFFSET - SIDE_LEN, coords);
    pushCoord3d(LEFT_OFFSET + SIDE_LEN, BOTTOM_OFFSET,            Z_OFFSET - SIDE_LEN, coords);
    pushCoord3d(LEFT_OFFSET,            BOTTOM_OFFSET,            Z_OFFSET - SIDE_LEN, coords);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * coords.size(), coords.data());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // left
    coords.clear();
    pushCoord3d(LEFT_OFFSET, BOTTOM_OFFSET + SIDE_LEN, Z_OFFSET - SIDE_LEN, coords);
    pushCoord3d(LEFT_OFFSET, BOTTOM_OFFSET + SIDE_LEN, Z_OFFSET,            coords);
    pushCoord3d(LEFT_OFFSET, BOTTOM_OFFSET,            Z_OFFSET - SIDE_LEN, coords);
    pushCoord3d(LEFT_OFFSET, BOTTOM_OFFSET,            Z_OFFSET,            coords);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * coords.size(), coords.data());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // top
    coords.clear();
    pushCoord3d(LEFT_OFFSET,            BOTTOM_OFFSET + SIDE_LEN, Z_OFFSET - SIDE_LEN, coords);
    pushCoord3d(LEFT_OFFSET + SIDE_LEN, BOTTOM_OFFSET + SIDE_LEN, Z_OFFSET - SIDE_LEN, coords);
    pushCoord3d(LEFT_OFFSET,            BOTTOM_OFFSET + SIDE_LEN, Z_OFFSET,            coords);
    pushCoord3d(LEFT_OFFSET + SIDE_LEN, BOTTOM_OFFSET + SIDE_LEN, Z_OFFSET,            coords);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * coords.size(), coords.data());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // bottom
    coords.clear();
    pushCoord3d(LEFT_OFFSET,            BOTTOM_OFFSET, Z_OFFSET,            coords);
    pushCoord3d(LEFT_OFFSET + SIDE_LEN, BOTTOM_OFFSET, Z_OFFSET,            coords);
    pushCoord3d(LEFT_OFFSET,            BOTTOM_OFFSET, Z_OFFSET - SIDE_LEN, coords);
    pushCoord3d(LEFT_OFFSET + SIDE_LEN, BOTTOM_OFFSET, Z_OFFSET - SIDE_LEN, coords);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * coords.size(), coords.data());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //glDisable(GL_DEPTH_TEST);
}

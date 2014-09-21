#include <windows.h>
#include <windowsx.h>

#include <algorithm>
#include <vector>

#include <GL/glew.h>
#include <GL/Wglew.h>

#include "Matrix4Df.h"
#include "Vector.h"
#include "Vec3Df.h"
#include "Projection.h"

#include "Camera.h"
#include "SwarmMember.h"
#include "GLPrograms.h"
#include "Utils.h"
#include "FrameRateCounter.h"

// Windows globals, defines, and prototypes
WCHAR szAppName[] = L"Play1";
HWND  ghWnd;
HDC   ghDC;
HGLRC ghRC;

const int GRID_WIDTH = 800;
const int GRID_HEIGHT = 800;

const int GRID_BUFFER = 100;
const int WIN_WIDTH = GRID_WIDTH + (6 * GRID_BUFFER);
const int WIN_HEIGHT = GRID_HEIGHT + (3 * GRID_BUFFER);

const float MOVE_AMOUNT = 5;
const float SPIN_AMOUNT = degToRad(2.0f);

LONG WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL setupPixelFormat(HDC);
void doCleanup(HWND);
void CALLBACK DrawTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

GLvoid resize(int width, int height);
void redoModelViewMatrix();
void redoProjectionMatrix(int width, int height);
void initializeGL();
GLvoid drawScene();
void createSwarm(int width, int height);
void setupData(int width, int height);

const int SWARM_SIZE = 500;
//Swarm g_swarm;
std::vector<SwarmMember*> g_swarm;

GLPrograms g_programs;

GLuint g_membersVao;
GLuint g_membersVbo;

GLuint g_outlineVao;
GLuint g_outlineVbo;

GLuint g_dotVao;
GLuint g_dotVbo;

const UINT_PTR DRAW_TIMER_ID = 1;

Camera g_camera(
    vec3df::create(
        ((float)GRID_WIDTH / 2),
        ((float)GRID_HEIGHT / 2),
        ((float)GRID_HEIGHT / 2)),
    degToRad(0), 0);

mat4df::Mat4Df g_modelView;
mat4df::Mat4Df g_projection;

FrameRateCounter g_frameRateCounter(5000);

bool g_shiftPressed = false;

const bool CREATE_CONSOLE = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    WNDCLASS wndclass;

    // Register the frame class
    wndclass.style = 0;
    wndclass.lpfnWndProc = (WNDPROC)MainWndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, szAppName);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndclass.lpszMenuName = szAppName;
    wndclass.lpszClassName = szAppName;

    if (!::RegisterClass(&wndclass)) {
        return FALSE;
    }

    // Create the frame
    ghWnd = ::CreateWindow(szAppName,
        L"Play1",
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WIN_WIDTH,
        WIN_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL);

    // make sure window was created
    if (!ghWnd) {
        return FALSE;
    }

    // show and update main window
    ::ShowWindow(ghWnd, nCmdShow);

    ::UpdateWindow(ghWnd);

    if (CREATE_CONSOLE) {
        ::AllocConsole();
        freopen("conin$", "r", stdin);
        freopen("conout$", "w", stdout);
        freopen("conout$", "w", stderr);
    }

    while (::GetMessage(&msg, NULL, 0, 0)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
    return TRUE;
}

void CALLBACK DrawTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    ::KillTimer(hwnd, idEvent);

    const float TARGET_FPS = 60;
    const UINT TIME_PER_FRAME = (UINT)(1000 / TARGET_FPS);
    ::SetTimer(ghWnd, DRAW_TIMER_ID, TIME_PER_FRAME, DrawTimerProc);

    drawScene();
}

// main window procedure
LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LONG lRet = 1;
    PAINTSTRUCT ps;
    RECT rect;

    switch (uMsg) {

    case WM_CREATE:
        ghDC = GetDC(hWnd);
        if (!setupPixelFormat(ghDC)) {
            PostQuitMessage(0);
        }

        initializeGL();
        GetClientRect(hWnd, &rect);
        resize(rect.right, rect.bottom); // added in leiu of passing dims to initialize
        g_programs.compilePrograms();

        createSwarm(GRID_WIDTH, GRID_HEIGHT);
        setupData(rect.right, rect.bottom);

        ::SetTimer(ghWnd, DRAW_TIMER_ID, 0, DrawTimerProc);

        break;

    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_SIZE:
        GetClientRect(hWnd, &rect);
        resize(rect.right, rect.bottom);
        break;

    case WM_CLOSE:
        doCleanup(hWnd);
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        doCleanup(hWnd);
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        switch (wParam) {
        case 'W':
            if (g_shiftPressed) {
                g_camera.moveUp(MOVE_AMOUNT);
            }
            else {
                g_camera.moveForward(MOVE_AMOUNT);
            }
            redoModelViewMatrix();
            break;

        case 'A':
            g_camera.moveSide(-MOVE_AMOUNT);
            redoModelViewMatrix();
            break;

        case 'S':
            if (g_shiftPressed) {
                g_camera.moveUp(-MOVE_AMOUNT);
            }
            else {
                g_camera.moveForward(-MOVE_AMOUNT);
            }
            redoModelViewMatrix();
            break;

        case 'D':
            g_camera.moveSide(MOVE_AMOUNT);
            redoModelViewMatrix();
            break;

        case 'Q':
            g_camera.spinAroundFwd(SPIN_AMOUNT);
            redoModelViewMatrix();
            break;

        case 'E':
            g_camera.spinAroundFwd(-SPIN_AMOUNT);
            redoModelViewMatrix();
            break;

        case VK_UP:
            g_camera.spinAroundSide(-SPIN_AMOUNT);
            redoModelViewMatrix();
            break;

        case VK_DOWN:
            g_camera.spinAroundSide(SPIN_AMOUNT);
            redoModelViewMatrix();
            break;

        case VK_LEFT:
            //if (g_shiftPressed) {
            //    g_camera.spinAroundFwd(SPIN_AMOUNT);
            //}
            //else {
                g_camera.spinAroundUp(SPIN_AMOUNT);
            //}
            redoModelViewMatrix();
            break;

        case VK_RIGHT:
            //if (g_shiftPressed) {
            //    g_camera.spinAroundFwd(-SPIN_AMOUNT);
            //}
            //else {
                g_camera.spinAroundUp(-SPIN_AMOUNT);
            //}
            redoModelViewMatrix();
            break;

        case VK_SHIFT:
            g_shiftPressed = true;
            break;
        }
        break;

    case WM_KEYUP:
        switch (wParam) {
        case VK_SHIFT:
            g_shiftPressed = false;
            break;
        }
        break;

    case WM_MOUSEMOVE: {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        break;
    }

    default:
        lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }

    return lRet;
}

void doCleanup(HWND hWnd) {

    if (CREATE_CONSOLE) {
        ::FreeConsole();
    }

    glDeleteVertexArrays(1, &g_membersVao);
    glDeleteVertexArrays(1, &g_outlineVao);
    glDeleteVertexArrays(1, &g_dotVao);
    g_programs.cleanupPrograms();

    if (ghRC) {
        wglDeleteContext(ghRC);
        ghRC = 0;
    }
    if (ghDC) {
        ReleaseDC(hWnd, ghDC);
        ghDC = 0;
    }
}

BOOL setupPixelFormat(HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd;
    int pixelformat;

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.dwLayerMask = PFD_MAIN_PLANE;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 0;// 16;
    pfd.cAccumBits = 0;
    pfd.cStencilBits = 0;

    pixelformat = ChoosePixelFormat(hdc, &pfd);

    if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == 0) {
        MessageBox(NULL, L"ChoosePixelFormat failed", L"Error", MB_OK);
        return FALSE;
    }

    if (!SetPixelFormat(hdc, pixelformat, &pfd)) {
        MessageBox(NULL, L"SetPixelFormat failed", L"Error", MB_OK);
        return FALSE;
    }

    return TRUE;
}

// OpenGL code

GLvoid resize(int width, int height) {
    //GLfloat aspect;

    glViewport(0, 0, width, height);

    //aspect = (GLfloat)width / height;

    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //gluPerspective(45.0, aspect, 3.0, 7.0);
    //glMatrixMode(GL_MODELVIEW);

    redoModelViewMatrix();
    redoProjectionMatrix(width, height);
}

void redoModelViewMatrix() {
    //g_modelView = projection::createLookAt(
    //    g_cameraPos, g_cameraTarget, g_cameraUp);
    g_modelView = projection::createLookAt(
        g_camera.getPosition(), g_camera.getTarget(), g_camera.getUp());
}

//void redoProjectionMatrix(int width, int height) {
//    float halfWidth = width / 2.0f;
//    float halfHeight = height / 2.0f;
//
//    const float NEAR_DIST = 0.5f;
//    const float FAR_DIST = max(width, height);
//
//    float sideClipRatio = NEAR_DIST / g_camera.getPosition()(0);
//    float widthDist = sideClipRatio * (width / 2.0f);
//    float heightDist = sideClipRatio * (height / 2.0f);
//
//    //g_projection = projection::createPerspective(-halfWidth, -halfHeight, halfWidth, halfHeight, NEAR_DIST, FAR_DIST);
//    g_projection = projection::createPerspective(-widthDist, -heightDist, widthDist, heightDist, NEAR_DIST, FAR_DIST);
//    //g_projection = projection::createOrtho(0, 0, (float)width, (float)height, 1, -1000);
//}

void redoProjectionMatrix(int width, int height) {
    const float NEAR_DIST = 0.5f;
    const float FAR_DIST = max((float)width, (float)height);
    const float FOV = 120.0f;

    g_projection = projection::createPerspective(FOV, (float)width, (float)height, NEAR_DIST, FAR_DIST);
}

void createSwarm(int width, int height) {
    for (int i = 0; i < SWARM_SIZE; i++) {
        SwarmMember* member = (i == 0) ?
            new SwarmMember(
                i + 1, 
                Position(
                    vec3df::create(
                        width / 2.0f,
                        height / 2.0f,
                        -1.0f),
                    0)) :
            new SwarmMember(
                i + 1,
                Position(
                    vec3df::create(
                        randf() * width,
                        randf() * height,
                        -1.0f),
                    randf() * (2 * (float)M_PI)));
        //g_swarm.addMember(member);
        Color color;
        color.r = rand() % 256;
        color.g = rand() % 256;
        color.b = rand() % 256;
        color.a = 255;
        member->setColor(color);
        g_swarm.push_back(member);
    }
}

void initializeGL() {
    HGLRC tempContext = wglCreateContext(ghDC);
    wglMakeCurrent(ghDC, tempContext);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        // failed to initialize GLEW!
    }

    // My card currently only supports OpenGL 4.1 -- jbl
    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,
        WGL_CONTEXT_FLAGS_ARB, 0,
        0
    };

    if (wglewIsSupported("WGL_ARB_create_context") == 1) {
        ghRC = wglCreateContextAttribsARB(ghDC, 0, attribs);
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(tempContext);
        wglMakeCurrent(ghDC, ghRC);
    }
    else {
        // It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
        ghRC = tempContext;
    }

    int OpenGLVersion[2];
    glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
}

void pushCoord(int x, int y, std::vector<GLfloat>& coords) {
    coords.push_back((GLfloat)x);
    coords.push_back((GLfloat)y);
    coords.push_back((GLfloat)0);
    coords.push_back((GLfloat)1);
}

GLuint g_gridPointCount;

void setupGrid() {
    const int GRID_SIZE = 100;
    std::vector<GLfloat> outlineCoords;

    int horizontalLineCount = (GRID_HEIGHT / GRID_SIZE) + 1;
    for (int i = 0; i < horizontalLineCount; i++) {
        int y = min(i * GRID_SIZE, GRID_HEIGHT);
        pushCoord(0, y, outlineCoords);
        pushCoord(GRID_WIDTH, y, outlineCoords);
    }

    int verticalLineCount = (GRID_WIDTH / GRID_SIZE) + 1;
    for (int i = 0; i < verticalLineCount; i++) {
        int x = min(i * GRID_SIZE, GRID_WIDTH);
        pushCoord(x, 0, outlineCoords);
        pushCoord(x, GRID_HEIGHT, outlineCoords);
    }

    g_gridPointCount = (horizontalLineCount + verticalLineCount) * 2;

    const GLuint OUTLINE_BUFFER_SIZE = sizeof(GLfloat) * outlineCoords.size();

    glGenVertexArrays(1, &g_outlineVao);
    glGenBuffers(1, &g_outlineVbo);
    glBindVertexArray(g_outlineVao);
    glBindBuffer(GL_ARRAY_BUFFER, g_outlineVbo);
    glBufferData(GL_ARRAY_BUFFER, OUTLINE_BUFFER_SIZE, outlineCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

GLuint g_dotPointCount;

void setupDot() {
    std::vector<GLfloat> dotCoords;

    const int DOT_RADIUS = 10;
    pushCoord(-DOT_RADIUS, 0, dotCoords);
    pushCoord(0, DOT_RADIUS * 2, dotCoords);
    pushCoord(DOT_RADIUS, 0, dotCoords);
    pushCoord(0, -DOT_RADIUS, dotCoords);
    pushCoord(-DOT_RADIUS, 0, dotCoords);

    g_dotPointCount = 5;

    const GLuint DOT_BUFFER_SIZE = sizeof(GLfloat) * dotCoords.size();

    glGenVertexArrays(1, &g_dotVao);
    glGenBuffers(1, &g_dotVbo);
    glBindVertexArray(g_dotVao);
    glBindBuffer(GL_ARRAY_BUFFER, g_dotVbo);
    glBufferData(GL_ARRAY_BUFFER, DOT_BUFFER_SIZE, dotCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

GLuint g_memberPosOffset;

void setupMembers() {

    GLfloat memberCoords[] = {
        -10, 10, 0, 1,
        0, -25, 0, 1,
        0, 0, 0, 1,
        10, 10, 0, 1
    };

    const GLuint MEMBER_COORDS_OFFSET = 0;
    const GLuint MEMBER_COORDS_SIZE = sizeof(memberCoords);
    const GLuint MEMBER_COLOR_OFFSET = MEMBER_COORDS_OFFSET + MEMBER_COORDS_SIZE;
    const GLuint MEMBER_COLOR_SIZE = sizeof(GLfloat) * 4 * g_swarm.size();
    const GLuint MEMBER_POS_OFFSET = MEMBER_COLOR_OFFSET + MEMBER_COLOR_SIZE;
    const GLuint MEMBER_POS_SIZE = sizeof(GLfloat) * 5 * g_swarm.size();
    const GLuint SWARM_BUFFER_SIZE = MEMBER_COORDS_SIZE + MEMBER_COLOR_SIZE + MEMBER_POS_SIZE;

    g_memberPosOffset = MEMBER_POS_OFFSET;

    glGenVertexArrays(1, &g_membersVao);
    glGenBuffers(1, &g_membersVbo);
    glBindVertexArray(g_membersVao);
    glBindBuffer(GL_ARRAY_BUFFER, g_membersVbo);
    glBufferData(GL_ARRAY_BUFFER, SWARM_BUFFER_SIZE, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, MEMBER_COORDS_OFFSET, MEMBER_COORDS_SIZE, memberCoords);

    float* colorPtr = (float*)((char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY) + MEMBER_COLOR_OFFSET);
    for (auto member : g_swarm) {
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

void setupData(int width, int height) {
    setupGrid();
    setupDot();
    setupMembers();
}

void drawGrid() {
    glUseProgram(g_programs.getSimpleProg());

    const GLuint PROG4_MODEL_VIEW_LOC = 0;
    const GLuint PROG4_PROJ_LOC = 1;
    const GLuint PROG4_COLOR_LOC = 2;
    glUniformMatrix4fv(PROG4_MODEL_VIEW_LOC, 1, GL_FALSE, g_modelView.getBuf());
    glUniformMatrix4fv(PROG4_PROJ_LOC, 1, GL_FALSE, g_projection.getBuf());
    glUniform4f(PROG4_COLOR_LOC, 1, 1, 0, 1);

    glBindVertexArray(g_outlineVao);
    glDrawArrays(GL_LINES, 0, g_gridPointCount);
}

void drawDot() {
    glUseProgram(g_programs.getSimpleProg());

    const GLuint PROG4_MODEL_VIEW_LOC = 0;
    const GLuint PROG4_PROJ_LOC = 1;
    const GLuint PROG4_COLOR_LOC = 2;
    glUniformMatrix4fv(PROG4_MODEL_VIEW_LOC, 1, GL_FALSE, g_modelView.getBuf());
    glUniformMatrix4fv(PROG4_PROJ_LOC, 1, GL_FALSE, g_projection.getBuf());
    glUniform4f(PROG4_COLOR_LOC, 1, 0, 0, 1);

    glBindVertexArray(g_dotVao);
    glDrawArrays(GL_LINE_STRIP, 0, g_dotPointCount);
}

void drawMembers() {
    glUseProgram(g_programs.getProg3());

    const GLuint PROG3_MODEL_VIEW_LOC = 0;
    const GLuint PROG3_PROJ_LOC = 1;
    glUniformMatrix4fv(PROG3_MODEL_VIEW_LOC, 1, GL_FALSE, g_modelView.getBuf());
    glUniformMatrix4fv(PROG3_PROJ_LOC, 1, GL_FALSE, g_projection.getBuf());

    float* posPtr = (float*)((char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY) + g_memberPosOffset);
    for (auto member : g_swarm) {
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

    glBindVertexArray(g_membersVao);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, SWARM_SIZE);
}

unsigned int g_lastFrameRatePrintTime = 0;
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //vec4df::Vec4Df v = vec4df::create(400, 400, 0, 1);
    //vec4df::Vec4Df mv = mat4df::mul(g_modelView, v);
    //vec4df::Vec4Df proj = mat4df::mul(g_projection, mv);

    drawGrid();
    drawDot();
    drawMembers();

    DWORD currentTime = timeGetTime();
    float frameRate = g_frameRateCounter.incorportateTime(currentTime);
    if ((currentTime - g_lastFrameRatePrintTime) > 1000) {
        //printf("fps: %f\n", frameRate);
        g_lastFrameRatePrintTime = currentTime;
    }

    ::SwapBuffers(ghDC);
}

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

#include "GLPrograms.h"
#include "DrawnGrid.h"
#include "OriginDot.h"
#include "DrawnSwarm.h"
#include "AxisDisplay.h"

#include "Camera.h"
#include "SwarmMember.h"
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
const float SPIN_AMOUNT = degToRad(5.0f);

LONG WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL setupPixelFormat(HDC);
void doCleanup(HWND);
void CALLBACK DrawTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

GLvoid resize(int width, int height);
void redoModelViewMatrix();
void redoProjectionMatrix(int width, int height);
void initializeGL();
GLvoid drawScene(int width, int height);
void createSwarm(int width, int height);
void setupData(int width, int height);

const int SWARM_SIZE = 500;

GLPrograms g_programs;

DrawnGrid g_drawnGrid(GRID_WIDTH, GRID_HEIGHT);
OriginDot g_originDot;
DrawnSwarm g_drawnSwarm(GRID_WIDTH, GRID_HEIGHT, SWARM_SIZE);
AxisDisplay g_axisDisplay;

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
bool g_paused = false;

const bool CREATE_CONSOLE = true;

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

DWORD g_lastTime = 0;
void CALLBACK DrawTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    if (g_paused) {
        return;
    }

    ::KillTimer(hwnd, idEvent);

    DWORD currentTime = timeGetTime();

    const float TARGET_FPS = 60;
    const UINT TIME_PER_FRAME = (UINT)(1000 / TARGET_FPS);

    DWORD nextTime = (g_lastTime > 0) ?
        (g_lastTime + TIME_PER_FRAME) : (currentTime + TIME_PER_FRAME);
    DWORD delay = (nextTime >= currentTime) ? (nextTime - currentTime) : 0;
    ::SetTimer(hwnd, DRAW_TIMER_ID, delay, DrawTimerProc);

    //if (g_lastTime > 0) {
    //    printf("time: %u\n", currentTime - g_lastTime);
    //}
    g_lastTime = currentTime;

    RECT rect;
    ::GetWindowRect(hwnd, &rect);
    drawScene(rect.right - rect.left, rect.bottom - rect.top);
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

        //createSwarm(GRID_WIDTH, GRID_HEIGHT);
        setupData(rect.right, rect.bottom);

        ::SetTimer(hWnd, DRAW_TIMER_ID, 0, DrawTimerProc);
        //::KillTimer(hWnd, DRAW_TIMER_ID);

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
        case 'P':
            g_paused = !g_paused;
            break;

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

    g_drawnGrid.cleanup();
    g_originDot.cleanup();
    g_drawnSwarm.cleanup();
    g_axisDisplay.cleanup();

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

void setupData(int width, int height) {

    g_drawnGrid.setProgram(g_programs.getSimpleProg());
    g_drawnGrid.setup();

    g_originDot.setProgram(g_programs.getSimpleProg());
    g_originDot.setup();

    g_drawnSwarm.setProgram(g_programs.getProg3());
    g_drawnSwarm.setup();

    g_axisDisplay.setProgram(g_programs.get2dProg());
    g_axisDisplay.setup();
}

unsigned int g_lastFrameRatePrintTime = 0;
void drawScene(int width, int height) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //vec4df::Vec4Df v = vec4df::create(400, 400, 0, 1);
    //vec4df::Vec4Df mv = mat4df::mul(g_modelView, v);
    //vec4df::Vec4Df proj = mat4df::mul(g_projection, mv);

    g_drawnGrid.draw(g_modelView, g_projection);
    g_originDot.draw(g_modelView, g_projection);
    g_drawnSwarm.draw(g_modelView, g_projection);
    g_axisDisplay.draw(width, height, g_camera);

    DWORD currentTime = timeGetTime();
    float frameRate = g_frameRateCounter.incorportateTime(currentTime);
    if ((currentTime - g_lastFrameRatePrintTime) > 1000) {
        printf("fps: %f\n", frameRate);
        g_lastFrameRatePrintTime = currentTime;
    }

    ::SwapBuffers(ghDC);
}

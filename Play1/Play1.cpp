#include <windows.h>
#include <windowsx.h>

#include <GL/glew.h>
#include <GL/Wglew.h>

#include "Swarm.h"
#include "GLPrograms.h"

// Windows globals, defines, and prototypes
WCHAR szAppName[] = L"Play1";
HWND  ghWnd;
HDC   ghDC;
HGLRC ghRC;

const int WIDTH = 1200;
const int HEIGHT = 800;

LONG WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL setupPixelFormat(HDC);
void doCleanup(HWND);
void CALLBACK DrawTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

GLvoid resize(GLsizei, GLsizei);
void initializeGL();
GLvoid drawScene();

const int SWARM_SIZE = 2;
Swarm g_swarm;

GLPrograms g_programs;
GLuint g_vaoID[1];

const UINT_PTR DRAW_TIMER_ID = 1;

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
        WIDTH,
        HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL);

    // make sure window was created
    if (!ghWnd) {
        return FALSE;
    }

    // show and update main window
    ShowWindow(ghWnd, nCmdShow);

    UpdateWindow(ghWnd);

    ::SetTimer(ghWnd, DRAW_TIMER_ID, 0, DrawTimerProc);

    while (::GetMessage(&msg, NULL, 0, 0)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
    return TRUE;
}

void CALLBACK DrawTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    const int TARGET_FPS = 60;
    const UINT TIME_PER_FRAME = 1000 / TARGET_FPS;
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

    //case WM_KEYDOWN:
    //    switch (wParam) {
    //    case VK_LEFT:
    //        longinc += 0.5F;
    //        break;

    //    case VK_RIGHT:
    //        longinc -= 0.5F;
    //        break;

    //    case VK_UP:
    //        latinc += 0.5F;
    //        break;

    //    case VK_DOWN:
    //        latinc -= 0.5F;
    //        break;
    //    }

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

    glDeleteVertexArrays(1, &g_vaoID[0]);
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

GLvoid resize(GLsizei width, GLsizei height) {
    //GLfloat aspect;

    glViewport(0, 0, width, height);

    //aspect = (GLfloat)width / height;

    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //gluPerspective(45.0, aspect, 3.0, 7.0);
    //glMatrixMode(GL_MODELVIEW);
}

void createSwarm() {
    //for (int i = 0; i < SWARM_SIZE; i++) {
    //    g_swarm.addMember(new SwarmMember())
    //}
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
        //It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
        ghRC = tempContext;
    }

    int OpenGLVersion[2];
    glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

    glGenVertexArrays(1, &g_vaoID[0]);
    glBindVertexArray(g_vaoID[0]);
}

GLvoid drawScene() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(g_programs.getProg1());

    DWORD currentTime = timeGetTime();
    float timeInSec = (float)currentTime / 1000.0f;

    GLfloat offset[] = {
        cos(timeInSec) * 0.5,
        sin(timeInSec) * 0.5,
        0.0f, 0.0f
    };
    GLfloat color[] = {
        cos(timeInSec) * 0.5f + 0.5f,
        -cos(timeInSec / 2.0f) * 0.5f + 0.5f,
        -cos(timeInSec / 3.0f) * 0.5f + 0.5f,
        1
    };

    glVertexAttrib4fv(0, offset);
    glVertexAttrib4fv(1, color);

    glDrawArrays(GL_TRIANGLES, 0, 3); // draw first object

    ::SwapBuffers(ghDC);
}

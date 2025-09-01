// WindowsPlatform.cpp - Windows平台抽象层实现

#include "WindowsPlatform.h"
#include <cassert>

HWND WindowsPlatform::s_hWnd = nullptr;
HINSTANCE WindowsPlatform::s_hInstance = nullptr;
bool WindowsPlatform::s_isRunning = false;

bool WindowsPlatform::s_keys[256] = { false };
bool WindowsPlatform::s_mouseButtons[3] = { false };
int WindowsPlatform::s_mouseX = 0;
int WindowsPlatform::s_mouseY = 0;

bool WindowsPlatform::CreateApplicationWindow(const std::wstring& title, int width, int height) {
    s_hInstance = GetModuleHandle(nullptr);

    WNDCLASSW wc = {};  // 使用 WNDCLASSW 而不是 WNDCLASS
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = s_hInstance;
    wc.lpszClassName = L"SoftRendererWindowClass";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (!RegisterClassW(&wc)) {  // 使用 RegisterClassW
        return false;
    }

    RECT rect = { 0, 0, width, height };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    s_hWnd = CreateWindowExW(  // 使用 CreateWindowExW
        0,
        L"SoftRendererWindowClass",
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        s_hInstance,
        nullptr
    );

    if (!s_hWnd) {
        return false;
    }

    ShowWindow(s_hWnd, SW_SHOW);
    UpdateWindow(s_hWnd);

    s_isRunning = true;
    return true;
}
void WindowsPlatform::DestroyApplicationWindow() {
    if (s_hWnd) {
        DestroyWindow(s_hWnd);
        s_hWnd = nullptr;
    }
    UnregisterClassW(L"SoftRendererWindowClass", s_hInstance);  // 使用 UnregisterClassW
    s_isRunning = false;
}

bool WindowsPlatform::ProcessMessages() {
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            s_isRunning = false;
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return s_isRunning;
}

void* WindowsPlatform::GetWindowHandle() {
    return static_cast<void*>(s_hWnd);
}

double WindowsPlatform::GetHighResolutionTime() {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return static_cast<double>(counter.QuadPart) / static_cast<double>(frequency.QuadPart);
}

bool WindowsPlatform::IsKeyPressed(int keyCode) {
    if (keyCode >= 0 && keyCode < 256) {
        return s_keys[keyCode];
    }
    return false;
}

bool WindowsPlatform::IsMouseButtonPressed(int button) {
    if (button >= 0 && button < 3) {
        return s_mouseButtons[button];
    }
    return false;
}

void WindowsPlatform::GetMousePosition(int& x, int& y) {
    x = s_mouseX;
    y = s_mouseY;
}

void* WindowsPlatform::CreateBackBuffer(int width, int height) {
    // 这里可以创建一个DIB section作为软件渲染的后缓冲区
    // 实现略...
    return nullptr;
}

void WindowsPlatform::DestroyBackBuffer(void* buffer) {
    // 释放后缓冲区资源
    // 实现略...
}

void WindowsPlatform::PresentBackBuffer(void* buffer) {
    // 将后缓冲区内容呈现到窗口
    // 实现略...
}

int WindowsPlatform::GetScreenWidth() {
    return GetSystemMetrics(SM_CXSCREEN);
}

int WindowsPlatform::GetScreenHeight() {
    return GetSystemMetrics(SM_CYSCREEN);
}

LRESULT CALLBACK WindowsPlatform::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            
        case WM_KEYDOWN:
            if (wParam >= 0 && wParam < 256) {
                s_keys[wParam] = true;
            }
            break;
            
        case WM_KEYUP:
            if (wParam >= 0 && wParam < 256) {
                s_keys[wParam] = false;
            }
            break;
            
        case WM_LBUTTONDOWN:
            s_mouseButtons[0] = true;
            break;
            
        case WM_LBUTTONUP:
            s_mouseButtons[0] = false;
            break;
            
        case WM_RBUTTONDOWN:
            s_mouseButtons[1] = true;
            break;
            
        case WM_RBUTTONUP:
            s_mouseButtons[1] = false;
            break;
            
        case WM_MBUTTONDOWN:
            s_mouseButtons[2] = true;
            break;
            
        case WM_MBUTTONUP:
            s_mouseButtons[2] = false;
            break;
            
        case WM_MOUSEMOVE:
            s_mouseX = GET_X_LPARAM(lParam);
            s_mouseY = GET_Y_LPARAM(lParam);
            break;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
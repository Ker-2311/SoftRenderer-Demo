// WindowsPlatform.h - Windows平台抽象层

#pragma once

#ifndef WINDOWS_PLATFORM_H
#define WINDOWS_PLATFORM_H

#include <windows.h>
#include <string>
#include <windowsx.h>

using namespace std;

class WindowsPlatform {
public:
    // 窗口创建和管理
    static bool CreateApplicationWindow(const std::wstring& title, int width, int height);
    static void DestroyApplicationWindow();
    static bool ProcessMessages();
    static void* GetWindowHandle();
    
    // 时间管理
    static double GetHighResolutionTime();
    
    // 输入处理
    static bool IsKeyPressed(int keyCode);
    static bool IsMouseButtonPressed(int button);
    static void GetMousePosition(int& x, int& y);
    
    // 图形缓冲区
    static void* CreateBackBuffer(int width, int height);
    static void DestroyBackBuffer(void* buffer);
    static void PresentBackBuffer(void* buffer);
    
    // 系统信息
    static int GetScreenWidth();
    static int GetScreenHeight();
    
private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    static HWND s_hWnd;
    static HINSTANCE s_hInstance;
    static bool s_isRunning;
    
    // 输入状态
    static bool s_keys[256];
    static bool s_mouseButtons[3];
    static int s_mouseX, s_mouseY;
};

#endif // WINDOWS_PLATFORM_H
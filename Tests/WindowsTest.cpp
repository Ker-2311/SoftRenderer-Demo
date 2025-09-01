// 主程序示例
#include "Windows/WindowsPlatform.h"
#include <iostream>

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
    {
    if (!WindowsPlatform::CreateApplicationWindow(L"Soft Renderer", 2000, 1000)) {
        std::cerr << "Failed to create application window!" << std::endl;
        return -1;
    }
    
    double lastTime = WindowsPlatform::GetHighResolutionTime();
    
    while (WindowsPlatform::ProcessMessages()) {
        double currentTime = WindowsPlatform::GetHighResolutionTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // 渲染逻辑...
        
        if (WindowsPlatform::IsKeyPressed(VK_ESCAPE)) {
            break;
        }
    }
    
    WindowsPlatform::DestroyApplicationWindow();
    return 0;
}
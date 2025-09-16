// 主程序示例
#include "Windows/WindowsPlatform.h"
#include "Core/Renderer.h"
#include <iostream>
#include "Core/Mesh.h"
#include "Shaders/DiffuseShader.h"
#include "Tools/MeshTool.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    // 16：9
    int width = 1600;
    int height = 900;
    if (!WindowsPlatform::CreateApplicationWindow(L"Soft Renderer", width, height))
    {
        std::cerr << "Failed to create application window!" << std::endl;
        return -1;
    }

    double lastTime = WindowsPlatform::GetHighResolutionTime();
    HWND hwnd = WindowsPlatform::GetHWND();
    HDC hdc = GetDC(hwnd);
    Camera *camera = new Camera(Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0)));
    Renderer renderer(hdc, *camera);
    // renderer.DrawLine(0,0,1000,1000,RGB(0,255,255),RGB(255,0,0));
    // renderer.DrawTriangle(width / 2, height / 4, width / 4, height * 3 / 4, width * 3 / 4, height * 3 / 4, RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255));

    Mesh *cubeMesh = MeshTool::CreateCubeMesh(Vector3f(0, 2, 5), Vector3f(30, 45, 0));
    Mesh *TrMesh = MeshTool::CreateTetrahedronMesh(Vector3f(0, 0, 5), Vector3f(0, 30, 30));

    while (WindowsPlatform::ProcessMessages())
    {
        double currentTime = WindowsPlatform::GetHighResolutionTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // 渲染逻辑...

        if (WindowsPlatform::IsKeyPressed(VK_ESCAPE))
        {
            break;
        }
        else if (WindowsPlatform::IsKeyPressed(VK_SPACE))
        {
            // 进行渲染
            renderer.Pass(*cubeMesh);
            renderer.Pass(*TrMesh);
        }
    }

    WindowsPlatform::DestroyApplicationWindow();
    return 0;
}
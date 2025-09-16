// 主程序示例
#include "Windows/WindowsPlatform.h"
#include "Core/Renderer.h"
#include <iostream>
#include "Core/Mesh.h"

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
    Camera *camera = new Camera(Transform(Vector3f(1,2,1),Vector3f(60,60,60)));
    Renderer renderer(hdc, *camera);
    // renderer.DrawLine(0,0,1000,1000,RGB(0,255,255),RGB(255,0,0));
    // renderer.DrawTriangle(width / 2, height / 4, width / 4, height * 3 / 4, width * 3 / 4, height * 3 / 4, RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255));

    Mesh *cubeMesh = new Mesh();
    cubeMesh->transform = Transform(Vector3f(0, 0, 0),Vector3f(0,0,0));

    vector<Vertex> vertexBuffer(8);
    vertexBuffer[0] = Vertex(Vector3f(-1, -1, -1));  // 0: 左-下-前
    vertexBuffer[1] = Vertex(Vector3f( 1, -1, -1));  // 1: 右-下-前
    vertexBuffer[2] = Vertex(Vector3f( 1,  1, -1));  // 2: 右-上-前
    vertexBuffer[3] = Vertex(Vector3f(-1,  1, -1));  // 3: 左-上-前
    vertexBuffer[4] = Vertex(Vector3f(-1, -1,  1));  // 4: 左-下-后
    vertexBuffer[5] = Vertex(Vector3f( 1, -1,  1));  // 5: 右-下-后
    vertexBuffer[6] = Vertex(Vector3f( 1,  1,  1));  // 6: 右-上-后
    vertexBuffer[7] = Vertex(Vector3f(-1,  1,  1));  // 7: 左-上-后

    vector<Vector3i> indexBuffer(12);
    // 前
    indexBuffer[0] = Vector3i(0, 2, 1);  // 前-下-右（CCW）
    indexBuffer[1] = Vector3i(0, 3, 2);  // 前-上-左（CCW）
    // 右
    indexBuffer[2] = Vector3i(1, 6, 5);  // 右-下-后（CCW）
    indexBuffer[3] = Vector3i(1, 2, 6);  // 右-上-前（CCW）
    // 后
    indexBuffer[4] = Vector3i(5, 7, 4);  // 后-下-左（CCW）
    indexBuffer[5] = Vector3i(5, 6, 7);  // 后-上-右（CCW）
    // 左
    indexBuffer[6] = Vector3i(4, 3, 0);  // 左-下-前（CCW）
    indexBuffer[7] = Vector3i(4, 7, 3);  // 左-上-后（CCW）
    // 上
    indexBuffer[8] = Vector3i(3, 6, 2);  // 上-前-右（CCW）
    indexBuffer[9] = Vector3i(3, 7, 6);  // 上-后-左（CCW）
    // 下
    indexBuffer[10] = Vector3i(0, 5, 4); // 下-前-左（CCW）
    indexBuffer[11] = Vector3i(0, 1, 5); // 下-后-右（CCW）

    cubeMesh->vertexBuffer = vertexBuffer;
    cubeMesh->indexBuffer = indexBuffer;

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
        }
    }

    WindowsPlatform::DestroyApplicationWindow();
    return 0;
}
// 主程序示例
#include "Windows/WindowsPlatform.h"
#include "Core/Renderer.h"
#include <iostream>
#include "Core/Mesh.h"
#include "Shaders/DiffuseShader.h"

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

    Mesh *cubeMesh = new Mesh();
    cubeMesh->transform = Transform(Vector3f(0, 0, 5), Vector3f(35, 45, 0));

    vector<Vertex> vertexBuffer(24); // 每个顶点在不同面上有不同的法线，所以需要24个顶点（6面×4顶点）
    vector<Vector3i> indexBuffer(12);

    // 前表面 (法线指向 -Z)
    vertexBuffer[0] = Vertex(Vector3f(-1, -1, -1), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0, 0, -1));
    vertexBuffer[1] = Vertex(Vector3f(1, -1, -1), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0, 0, -1));
    vertexBuffer[2] = Vertex(Vector3f(1, 1, -1), Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0, 0, -1));
    vertexBuffer[3] = Vertex(Vector3f(-1, 1, -1), Vector3f(1.0f, 1.0f, 0.0f), Vector3f(0, 0, -1));
    indexBuffer[0] = Vector3i(0, 2, 1); // 前-下-右（CCW）
    indexBuffer[1] = Vector3i(0, 3, 2); // 前-上-左（CCW）

    // 右表面 (法线指向 +X)
    vertexBuffer[4] = Vertex(Vector3f(1, -1, -1), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(1, 0, 0));
    vertexBuffer[5] = Vertex(Vector3f(1, -1, 1), Vector3f(0.0f, 1.0f, 1.0f), Vector3f(1, 0, 0));
    vertexBuffer[6] = Vertex(Vector3f(1, 1, 1), Vector3f(1.0f, 1.0f, 1.0f), Vector3f(1, 0, 0));
    vertexBuffer[7] = Vertex(Vector3f(1, 1, -1), Vector3f(0.0f, 0.0f, 1.0f), Vector3f(1, 0, 0));
    indexBuffer[2] = Vector3i(4, 6, 5); // 右-下-后（CCW）
    indexBuffer[3] = Vector3i(4, 7, 6); // 右-上-前（CCW）

    // 后表面 (法线指向 +Z)
    vertexBuffer[8] = Vertex(Vector3f(1, -1, 1), Vector3f(0.0f, 1.0f, 1.0f), Vector3f(0, 0, 1));
    vertexBuffer[9] = Vertex(Vector3f(-1, -1, 1), Vector3f(1.0f, 0.0f, 1.0f), Vector3f(0, 0, 1));
    vertexBuffer[10] = Vertex(Vector3f(-1, 1, 1), Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0, 0, 1));
    vertexBuffer[11] = Vertex(Vector3f(1, 1, 1), Vector3f(1.0f, 1.0f, 1.0f), Vector3f(0, 0, 1));
    indexBuffer[4] = Vector3i(8, 10, 9);  // 后-下-左（CCW）
    indexBuffer[5] = Vector3i(8, 11, 10); // 后-上-右（CCW）

    // 左表面 (法线指向 -X)
    vertexBuffer[12] = Vertex(Vector3f(-1, -1, 1), Vector3f(1.0f, 0.0f, 1.0f), Vector3f(-1, 0, 0));
    vertexBuffer[13] = Vertex(Vector3f(-1, -1, -1), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(-1, 0, 0));
    vertexBuffer[14] = Vertex(Vector3f(-1, 1, -1), Vector3f(1.0f, 1.0f, 0.0f), Vector3f(-1, 0, 0));
    vertexBuffer[15] = Vertex(Vector3f(-1, 1, 1), Vector3f(0.5f, 0.5f, 0.5f), Vector3f(-1, 0, 0));
    indexBuffer[6] = Vector3i(12, 14, 13); // 左-下-前（CCW）
    indexBuffer[7] = Vector3i(12, 15, 14); // 左-上-后（CCW）

    // 上表面 (法线指向 +Y)
    vertexBuffer[16] = Vertex(Vector3f(-1, 1, -1), Vector3f(1.0f, 1.0f, 0.0f), Vector3f(0, 1, 0));
    vertexBuffer[17] = Vertex(Vector3f(1, 1, -1), Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0, 1, 0));
    vertexBuffer[18] = Vertex(Vector3f(1, 1, 1), Vector3f(1.0f, 1.0f, 1.0f), Vector3f(0, 1, 0));
    vertexBuffer[19] = Vertex(Vector3f(-1, 1, 1), Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0, 1, 0));
    indexBuffer[8] = Vector3i(16, 18, 17); // 上-前-右（CCW）
    indexBuffer[9] = Vector3i(16, 19, 18); // 上-后-左（CCW）

    // 下表面 (法线指向 -Y)
    vertexBuffer[20] = Vertex(Vector3f(-1, -1, 1), Vector3f(1.0f, 0.0f, 1.0f), Vector3f(0, -1, 0));
    vertexBuffer[21] = Vertex(Vector3f(1, -1, 1), Vector3f(0.0f, 1.0f, 1.0f), Vector3f(0, -1, 0));
    vertexBuffer[22] = Vertex(Vector3f(1, -1, -1), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0, -1, 0));
    vertexBuffer[23] = Vertex(Vector3f(-1, -1, -1), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0, -1, 0));
    indexBuffer[10] = Vector3i(20, 22, 21); // 下-前-左（CCW）
    indexBuffer[11] = Vector3i(20, 23, 22); // 下-后-右（CCW）

    cubeMesh->vertexBuffer = vertexBuffer;
    cubeMesh->indexBuffer = indexBuffer;
    cubeMesh->ps = make_shared<DiffusePS>();
    cubeMesh->vs = make_shared<DiffuseVS>();

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
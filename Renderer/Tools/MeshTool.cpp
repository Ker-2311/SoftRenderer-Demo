#include "MeshTool.h"

#define PI 3.1415926f
Mesh* MeshTool::CreateTetrahedronMesh(Vector3f pos, Vector3f rot)
{
    Mesh* tetraMesh = new Mesh();
    tetraMesh->transform = Transform(pos, rot);

    const float size = 1.0f;
    
    // 四面体有4个顶点
    vector<Vertex> vertexBuffer(4);
    
    // 四面体有4个三角形面
    vector<Vector3i> indexBuffer(4);

    // 计算顶点位置 (标准正四面体顶点坐标)
    // 正四面体顶点坐标计算公式
    const float a = size;
    const float sqrt3 = sqrtf(3.0f);
    const float sqrt6 = sqrtf(6.0f);
    
    // 四个顶点位置 (正四面体)
    Vector3f v0(0.0f, 0.0f, a);                           // 顶部顶点
    Vector3f v1(-a/sqrt3, 0.0f, -a/3.0f);                // 底部左前
    Vector3f v2(a/(2.0f*sqrt3), -a/2.0f, -a/3.0f);       // 底部右前
    Vector3f v3(a/(2.0f*sqrt3), a/2.0f, -a/3.0f);        // 底部后

    // 设置顶点位置和颜色
    vertexBuffer[0] = Vertex(v0, Vector3f(1.0f, 0.0f, 0.0f));  // 红色，顶部
    vertexBuffer[1] = Vertex(v1, Vector3f(0.0f, 1.0f, 0.0f));  // 绿色
    vertexBuffer[2] = Vertex(v2, Vector3f(0.0f, 0.0f, 1.0f));  // 蓝色
    vertexBuffer[3] = Vertex(v3, Vector3f(1.0f, 1.0f, 0.0f));  // 黄色

    // 设置索引 (4个三角形面)
    // 底面 (v1, v3, v2)
    indexBuffer[0] = Vector3i(1, 3, 2);
    
    // 前面 (v0, v2, v1)
    indexBuffer[1] = Vector3i(0, 2, 1);
    
    // 右面 (v0, v3, v2)
    indexBuffer[2] = Vector3i(0, 3, 2);
    
    // 左面 (v0, v1, v3)
    indexBuffer[3] = Vector3i(0, 1, 3);

    tetraMesh->vertexBuffer = vertexBuffer;
    tetraMesh->indexBuffer = indexBuffer;

    return tetraMesh;
}

Mesh* MeshTool::CreateIcosahedronMesh(Vector3f pos, Vector3f rot)
{
    Mesh* icosaMesh = new Mesh();
    icosaMesh->transform = Transform(pos, rot);

    // 正二十面体有12个顶点
    vector<Vertex> vertexBuffer(12);
    float radius = 1.0f;
    // 正二十面体有20个三角形面
    vector<Vector3i> indexBuffer(20);

    // 黄金比例
    const float t = (1.0f + sqrt(5.0f)) / 2.0f;
    const float tau = t / sqrt(1.0f + t * t);
    const float one = 1.0f / sqrt(1.0f + t * t);

    // 计算12个顶点位置 (基于黄金矩形)
    Vector3f vertices[12] = {
        Vector3f( tau,  one,  0.0f),    // 0
        Vector3f(-tau,  one,  0.0f),    // 1
        Vector3f(-tau, -one,  0.0f),    // 2
        Vector3f( tau, -one,  0.0f),     // 3
        Vector3f( one, 0.0f,  tau),     // 4
        Vector3f( one, 0.0f, -tau),     // 5
        Vector3f(-one, 0.0f, -tau),     // 6
        Vector3f(-one, 0.0f,  tau),     // 7
        Vector3f(0.0f,  tau,  one),     // 8
        Vector3f(0.0f, -tau,  one),     // 9
        Vector3f(0.0f, -tau, -one),     // 10
        Vector3f(0.0f,  tau, -one)      // 11
    };

    // 设置顶点位置、颜色和初始法线
    for (int i = 0; i < 12; ++i) {
        // 归一化并缩放顶点位置
        Vector3f pos = vertices[i].Normalized() * radius;
        // 设置颜色 (基于顶点位置)
        Vector3f color(0.5f + pos.x * 0.5f, 0.5f + pos.y * 0.5f, 0.5f + pos.z * 0.5f);
        vertexBuffer[i] = Vertex(pos, color, pos.Normalized());
    }

    // 设置20个三角形面的索引
    // 顶部五角锥 (5个面)
    indexBuffer[0] = Vector3i(0, 8, 4);
    indexBuffer[1] = Vector3i(0, 5, 11);
    indexBuffer[2] = Vector3i(0, 11, 8);
    indexBuffer[3] = Vector3i(0, 4, 5);
    indexBuffer[4] = Vector3i(8, 11, 7);
    
    // 中部环带 (10个面)
    indexBuffer[5] = Vector3i(4, 8, 7);
    indexBuffer[6] = Vector3i(5, 4, 3);
    indexBuffer[7] = Vector3i(11, 5, 10);
    indexBuffer[8] = Vector3i(7, 11, 6);
    indexBuffer[9] = Vector3i(8, 1, 7);
    indexBuffer[10] = Vector3i(1, 8, 11);
    indexBuffer[11] = Vector3i(3, 4, 9);
    indexBuffer[12] = Vector3i(10, 5, 3);
    indexBuffer[13] = Vector3i(6, 11, 10);
    indexBuffer[14] = Vector3i(7, 1, 6);
    
    // 底部五角锥 (5个面)
    indexBuffer[15] = Vector3i(1, 2, 6);
    indexBuffer[16] = Vector3i(2, 1, 10);
    indexBuffer[17] = Vector3i(6, 2, 10);
    indexBuffer[18] = Vector3i(3, 9, 10);
    indexBuffer[19] = Vector3i(2, 10, 9);

    // 计算每个面的法线并更新顶点法线
    Vector3f faceNormals[20];
    for (int i = 0; i < 20; ++i) {
        Vector3i face = indexBuffer[i];
        Vector3f v0 = vertexBuffer[face.x].pos;
        Vector3f v1 = vertexBuffer[face.y].pos;
        Vector3f v2 = vertexBuffer[face.z].pos;
        
        faceNormals[i] = (v1 - v0).Cross(v2 - v0).Normalized();
        
        // 更新每个顶点的法线
        vertexBuffer[face.x].normal = vertexBuffer[face.x].normal + faceNormals[i];
        vertexBuffer[face.y].normal = vertexBuffer[face.y].normal + faceNormals[i];
        vertexBuffer[face.z].normal = vertexBuffer[face.z].normal + faceNormals[i];
    }
    
    // 归一化所有顶点法线
    for (auto& vertex : vertexBuffer) {
        vertex.normal = vertex.normal.Normalized();
    }

    icosaMesh->vertexBuffer = vertexBuffer;
    icosaMesh->indexBuffer = indexBuffer;
    icosaMesh->ps = make_shared<DiffusePS>();
    icosaMesh->vs = make_shared<DiffuseVS>();

    return icosaMesh;
}

Mesh *MeshTool::CreateCubeMesh(Vector3f pos, Vector3f rot)
{
    Mesh *cubeMesh = new Mesh();
    cubeMesh->transform = Transform(pos, rot);

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

    return cubeMesh;
}
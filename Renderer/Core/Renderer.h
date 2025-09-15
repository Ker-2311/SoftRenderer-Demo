#pragma once
#include "Windows/WindowsPlatform.h"
#include "Vertex.h"
#include <vector>
#include "IShader.h"
#include <memory>

using namespace std;

// 装配的三角形图元
class Triangle
{
public:
    shared_ptr<BaseVertexOutput> V0,V1,V2;

    Triangle(shared_ptr<BaseVertexOutput> v0,shared_ptr<BaseVertexOutput> v1,shared_ptr<BaseVertexOutput> v2)
    : V0(v0),V1(v1),V2(v2) {}
};


class Renderer
{
public:
    Renderer(HDC hdc);

    // 顶点着色阶段(因为在CPU上渲染所以没有IA阶段)
    void VertexShaderStage(const vector<Vertex> &vertexList, vector<shared_ptr<BaseVertexOutput>> &outputList, shared_ptr<VertexShader> shader);
    // 图元装配阶段
    void PrimitiveAssembly(const vector<shared_ptr<BaseVertexOutput>> &vertexOutputList,const vector<Vector3i> &indexList,vector<shared_ptr<Triangle>> &primitiveList);
    // 裁切阶段
    void ClipStage(vector<shared_ptr<Triangle>> &primitiveList);
    // 光栅化阶段
    void Rasterize(const vector<shared_ptr<Triangle>> &primitiveList, vector<shared_ptr<PixelInput>> &outputList);

    void PixelShaderStage(const vector<shared_ptr<PixelInput>> &outputList);

    void DrawLine(int x0, int y0, int x1, int y1, COLORREF color0, COLORREF color1);
    // 通过重心坐标绘制三角形
    void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, COLORREF color0, COLORREF color1, COLORREF color2);

private:
    HDC m_hdc;
    int m_width;
    int m_height;
    // float m_zbuffer[][];

private:
    void DrawPixel(int x, int y, COLORREF color);
    //Sutherland-Hodgman 裁剪算法
    void ClipPolygon(vector<Vector4f> &polygon);
    // 检查三角形是否完全在视锥体外
    bool IsTriangleInsideFrustum(const Vector4f &v0, const Vector4f &v1, const Vector4f &v2);
    // 检查三角形是否完全在视锥体内
    bool IsTriangleOutsideFrustum(const Vector4f &v0, const Vector4f &v1, const Vector4f &v2);
    // 计算点P的三角形重心权重
    inline Vector3f CalculateBarycentric(int x0, int y0, int x1, int y1, int x2, int y2, int xp, int yp) noexcept;
    // 快速判断点P是否在三角形内
    inline bool PointInTriangleFastCheck(int x0, int y0, int x1, int y1, int x2, int y2, int xp, int yp) noexcept;
};
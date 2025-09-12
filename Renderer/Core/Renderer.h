#pragma once
#include "Windows/WindowsPlatform.h"
#include "Vertex.h"
#include <vector>
#include "IShader.h"
#include <memory>

using namespace std;

class Renderer
{
public:
    Renderer(HDC hdc);

    // 顶点着色阶段(因为在CPU上渲染所以没有IA阶段)
    void VertexShader(const vector<Vertex> &vertexList,vector<shared_ptr<BaseVertexOutput>> &outputList);
    // 光栅化阶段
    void Rasterize(const vector<shared_ptr<BaseVertexOutput>> &vertexOutputList);

    void DrawLine(int x0, int y0, int x1, int y1,COLORREF color0,COLORREF color1);
    // 通过重心坐标绘制三角形
    void DrawTriangle(int x0,int y0,int x1, int y1,int x2,int y2,COLORREF color0,COLORREF color1,COLORREF color2);

private:
    HDC _hdc;

private:
    void DrawPixel(int x,int y,COLORREF color);
    // 计算点P的三角形重心权重
    inline Vector3f CalculateBarycentric(int x0,int y0,int x1, int y1,int x2,int y2,int xp,int yp) noexcept;
    // 快速判断点P是否在三角形内
    inline bool PointInTriangleFastCheck(int x0, int y0, int x1, int y1, int x2, int y2, int xp, int yp) noexcept;
};
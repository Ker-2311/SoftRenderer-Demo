#pragma once
#include "Windows/WindowsPlatform.h"
#include "Vertex.h"
#include <vector>

using namespace std;

class Renderer
{
public:
    Renderer(HDC hdc);

    void DrawLine(int x0, int y0, int x1, int y1,COLORREF color0,COLORREF color1);
    // 通过重心坐标绘制三角形
    void DrawTriangle(int x0,int y0,int x1, int y1,int x2,int y2,COLORREF color0,COLORREF color1,COLORREF color2);

private:
    HDC _hdc;

private:
    void DrawPixel(int x,int y,COLORREF color);
    // 计算点P的三角形重心权重
    Vector3f CalculateBarycentric(int x0,int y0,int x1, int y1,int x2,int y2,int xp,int yp);
};
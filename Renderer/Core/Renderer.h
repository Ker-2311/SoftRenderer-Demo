#pragma once
#include "Windows/WindowsPlatform.h"
#include "Vertex.h"
#include <vector>

using namespace std;

class Renderer
{
public:
    Renderer(HDC hdc);

    void DrawLine(int x0, int y0, int x1, int y1,COLORREF color);


private:
    HDC _hdc;

private:
    void DrawPixel(int x,int y,COLORREF color);
};
#include "Renderer.h"

Renderer::Renderer(HDC hdc):_hdc(hdc)
{

}

void Renderer::DrawPixel(int x,int y,COLORREF color)
{
    SetPixel(_hdc,x,y,color);
}

void Renderer::DrawLine(int x0, int y0, int x1, int y1,COLORREF color)
{
    // 保证斜率小于1的情况下执行Bresenham算法
    bool steep = abs(y1 - y0) > abs(x1 - x0);

    // 如果斜率大于1，交换x和y
    if (steep)
    {
        swap(x0, y0);
        swap(x1, y1);
    }

    // 确保x0 < x1
    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = abs(y1 - y0);

    int error = dx / 2;
    int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;

    for (int x = x0; x <= x1; x++)
    {
        if (steep)
        {
            DrawPixel(y,x,color);
        }
        else
        {
            DrawPixel(x,y,color);
        }

        error -= dy;
        if (error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}

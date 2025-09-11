#include "Renderer.h"

Renderer::Renderer(HDC hdc) : _hdc(hdc)
{
}

void Renderer::DrawPixel(int x, int y, COLORREF color)
{
    SetPixel(_hdc, x, y, color);
}

void Renderer::DrawLine(int x0, int y0, int x1, int y1, COLORREF color0, COLORREF color1)
{
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep)
    {
        swap(x0, y0);
        swap(x1, y1);
    }

    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
        swap(color0, color1);
    }

    int dx = x1 - x0;
    int dy = abs(y1 - y0);

    int error = dx;
    int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;

    // 提取颜色分量
    BYTE r0 = GetRValue(color0);
    BYTE g0 = GetGValue(color0);
    BYTE b0 = GetBValue(color0);

    BYTE r1 = GetRValue(color1);
    BYTE g1 = GetGValue(color1);
    BYTE b1 = GetBValue(color1);

    // 使用整数运算进行颜色插值
    int steps = x1 - x0;
    int current_step = 0;

    for (int x = x0; x <= x1; x++)
    {
        // 颜色插值计算
        BYTE r = r0 + (current_step * (r1 - r0)) / steps;
        BYTE g = g0 + (current_step * (g1 - g0)) / steps;
        BYTE b = b0 + (current_step * (b1 - b0)) / steps;

        COLORREF color = RGB(r, g, b);

        if (steep)
        {
            DrawPixel(y, x, color);
        }
        else
        {
            DrawPixel(x, y, color);
        }

        error -= 2 * dy;
        if (error < 0)
        {
            y += ystep;
            error += 2 * dx;
        }

        current_step++;
    }
}

void Renderer::DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, COLORREF color0, COLORREF color1, COLORREF color2)
{
    // 创建包围盒
    int xMax = max(max(x0, x1), x2);
    int yMax = max(max(y0, y1), y2);
    int xMin = min(min(x0, x1), x2);
    int yMin = min(min(y0, y1), y2);

    // 提取颜色分量
    BYTE r0 = GetRValue(color0);
    BYTE g0 = GetGValue(color0);
    BYTE b0 = GetBValue(color0);

    BYTE r1 = GetRValue(color1);
    BYTE g1 = GetGValue(color1);
    BYTE b1 = GetBValue(color1);

    BYTE r2 = GetRValue(color2);
    BYTE g2 = GetGValue(color2);
    BYTE b2 = GetBValue(color2);

    for (int j = yMin; j <= yMax; j++)
    {
        for (int i = xMin; i <= xMax; i++)
        {
            Vector3f baryCentric = CalculateBarycentric(x0, y0, x1, y1, x2, y2, i, j);
            bool isInside = (baryCentric.x >= -1e-6f) && (baryCentric.y >= -1e-6f) && (baryCentric.z >= -1e-6f);
            if (!isInside)
                continue;

            BYTE r = static_cast<BYTE>(r0 * baryCentric.x + r1 * baryCentric.y + r2 * baryCentric.z);
            BYTE g = static_cast<BYTE>(g0 * baryCentric.x + g1 * baryCentric.y + g2 * baryCentric.z);
            BYTE b = static_cast<BYTE>(b0 * baryCentric.x + b1 * baryCentric.y + b2 * baryCentric.z);

            COLORREF color = RGB(r,g,b);
            DrawPixel(i,j,color);
        }
    }
}

Vector3f Renderer::CalculateBarycentric(int x0, int y0, int x1, int y1, int x2, int y2, int xp, int yp)
{
    Vector2i v0(x1 - x0, y1 - y0);
    Vector2i v1(x2 - x0, y2 - y0);
    Vector2i vp(xp - x0, yp - y0);

    // 解线性方程
    int d00 = v0.Dot(v0);
    int d01 = v0.Dot(v1);
    int d10 = v1.Dot(v0);
    int d11 = v1.Dot(v1);
    int c0 = vp.Dot(v0);
    int c1 = vp.Dot(v1);

    float det = d11 * d00 - d01 * d10;
    float det1 = d11 * c0 - d01 * c1;
    float det2 = c1 * d00 - c0 * d10;

    // 无解
    if (std::abs(det) < 1e-6f)
    {
        return Vector3f(-1, -1, -1); // 返回无效坐标
    }

    // 克拉默法则
    float beta = det1 / det;
    float gamma = det2 / det;
    float alpha = 1 - beta - gamma;
    return Vector3f(alpha, beta, gamma);
}
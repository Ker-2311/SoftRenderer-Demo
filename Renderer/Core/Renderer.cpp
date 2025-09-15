#include "Renderer.h"

Renderer::Renderer(HDC hdc) : m_hdc(hdc)
{
    m_width = GetDeviceCaps(hdc, HORZRES);
    m_height = GetDeviceCaps(hdc, VERTRES);
    m_zbuffer.resize(m_height,vector<float>(m_width,1));
}

void Renderer::VertexShaderStage(const vector<Vertex> &inputList, vector<shared_ptr<BaseVertexOutput>> &outputList, shared_ptr<VertexShader> shader)
{
    outputList.resize(inputList.size());
    for (int i = 0; i < inputList.size(); i++)
    {
        shared_ptr<BaseVertexOutput> vertexOutput = shader->Process(inputList[i]);
        // 透视除法
        float w = vertexOutput->oPos.w;
        vertexOutput->oPos = vertexOutput->oPos / w;
        // 保存w值用于透视矫正
        vertexOutput->oPos.w = w;
        outputList[i] = vertexOutput;
    }
}

void Renderer::PrimitiveAssembly(const vector<shared_ptr<BaseVertexOutput>> &vertexOutputList, const vector<Vector3i> &indexList, vector<shared_ptr<Triangle>> &primitiveList)
{
    primitiveList.resize(indexList.size());
    for (int i = 0; i < indexList.size(); i++)
    {
        Vector3i index = indexList[i];
        shared_ptr<BaseVertexOutput> v0 = vertexOutputList[index.x];
        shared_ptr<BaseVertexOutput> v1 = vertexOutputList[index.y];
        shared_ptr<BaseVertexOutput> v2 = vertexOutputList[index.z];
        primitiveList[i] = make_shared<Triangle>(v0, v1, v2);
    }
}

void Renderer::ClipStage(vector<shared_ptr<Triangle>> &primitiveList)
{
    vector<shared_ptr<Triangle>> clippedTriangles;

    for (const auto &triangle : primitiveList)
    {
        auto v0 = triangle->V0;
        auto v1 = triangle->V1;
        auto v2 = triangle->V2;

        // 检查三角形是否完全在视锥体外
        if (IsTriangleOutsideFrustum(v0->oPos, v1->oPos, v2->oPos))
        {
            continue; // 丢弃
        }

        // 检查是否需要裁剪
        if (IsTriangleInsideFrustum(v0->oPos, v1->oPos, v2->oPos))
        {
            clippedTriangles.push_back(triangle); // 完全在视锥体内，直接保留
        }
        else
        {
            // 进行裁剪（Sutherland-Hodgman 算法）
            vector<Vector4f> polygon = {v0->oPos, v1->oPos, v2->oPos};
            ClipPolygon(polygon);

            // 将裁剪后的多边形拆分成三角形
            for (size_t i = 1; i + 1 < polygon.size(); i++)
            {
                auto newV0 = make_shared<BaseVertexOutput>(*v0);
                auto newV1 = make_shared<BaseVertexOutput>(*v1);
                auto newV2 = make_shared<BaseVertexOutput>(*v2);

                newV0->oPos = polygon[0];
                newV1->oPos = polygon[i];
                newV2->oPos = polygon[i + 1];

                clippedTriangles.push_back(make_shared<Triangle>(newV0, newV1, newV2));
            }
        }
    }

    primitiveList = std::move(clippedTriangles); // 替换为裁剪后的三角形
}

void Renderer::Rasterize(const vector<shared_ptr<Triangle>> &primitiveList, vector<shared_ptr<PixelInput>> &outputList)
{
    for (int i = 0; i < primitiveList.size(); i++)
    {
        shared_ptr<Triangle> triangle = primitiveList[i];
        // TODO: 背面剔除

        // 透视矫正插值
        shared_ptr<BaseVertexOutput> v0 = triangle->V0;
        shared_ptr<BaseVertexOutput> v1 = triangle->V1;
        shared_ptr<BaseVertexOutput> v2 = triangle->V2;

        // 转换为屏幕坐标（Y轴翻转）
        int x0 = static_cast<int>((v0->oPos.x + 1.0f) * m_width / 2.0f);
        int y0 = static_cast<int>((1.0f - v0->oPos.y) * m_height / 2.0f); // 注意：1-y 翻转Y轴
        int x1 = static_cast<int>((v1->oPos.x + 1.0f) * m_width / 2.0f);
        int y1 = static_cast<int>((1.0f - v1->oPos.y) * m_height / 2.0f);
        int x2 = static_cast<int>((v2->oPos.x + 1.0f) * m_width / 2.0f);
        int y2 = static_cast<int>((1.0f - v2->oPos.y) * m_height / 2.0f);

        // 创建包围盒   
        int xMax = max(max(x0, x1), x2);
        int yMax = max(max(y0, y1), y2);
        int xMin = min(min(x0, x1), x2);
        int yMin = min(min(y0, y1), y2);

        // 确保在屏幕范围内
        xMin = max(xMin, 0);
        yMin = max(yMin, 0);
        xMax = min(xMax, m_width - 1);
        yMax = min(yMax, m_height - 1);

        // 遍历包围盒中的每个像素
        for (int y = yMin; y <= yMax; y++)
        {
            for (int x = xMin; x <= xMax; x++)
            {
                // 快速检查是否在三角形内
                if (!PointInTriangleFastCheck(x0, y0, x1, y1, x2, y2, x, y))
                    continue;

                // 计算重心坐标
                Vector3f bary = CalculateBarycentric(x0, y0, x1, y1, x2, y2, x, y);
                float alpha = bary.x;
                float beta = bary.y;
                float gamma = bary.z;

                // 计算1/w的插值
                float invW0 = 1.0f / v0->oPos.w;
                float invW1 = 1.0f / v1->oPos.w;
                float invW2 = 1.0f / v2->oPos.w;
                float invW = alpha * invW0 + beta * invW1 + gamma * invW2;
                float w = 1.0f / invW;

                // 创建像素输入数据
                auto pixelInput = make_shared<PixelInput>();
                pixelInput->iPos = Vector2i(x, y);
                pixelInput->depth = alpha * v0->oPos.z + beta * v1->oPos.z + gamma * v2->oPos.z;

                // 透视矫正插值所有顶点属性
                for (auto &[type, attr] : v0->attributes)
                {
                    // 获取三个顶点的属性值
                    Vector4f attr0 = v0->attributes[type];
                    Vector4f attr1 = v1->attributes[type];
                    Vector4f attr2 = v2->attributes[type];

                    // 透视矫正插值
                    Vector4f interpolated = (attr0 * invW0 * alpha + attr1 * beta * invW1 + attr2 * gamma * invW2) * w;
                    pixelInput->attributes[type] = interpolated;
                }

                outputList.push_back(pixelInput);
            }
        }
    }
}

void Renderer::PixelShaderStage(const vector<shared_ptr<PixelInput>> &inputList, vector<shared_ptr<BasePixelOutput>> &outputList, shared_ptr<PixelShader> shader)
{
    outputList.resize(inputList.size());
    for (int i = 0; i < inputList.size(); i++)
    {
        shared_ptr<BasePixelOutput> pixelOutput = shader->Process(*inputList[i]);
        outputList[i] = pixelOutput;
    }
}

void Renderer::OutputDraw(const vector<shared_ptr<BasePixelOutput>> &outputList)
{
    for (auto output : outputList)
    {
        // 确保颜色输出
        if (output->attributes.count(AttributeType::Color) > 0)
        {
            Vector2i pos = output->oPos;
            Vector4f color = output->attributes[AttributeType::Color];
            // 深度测试
            if (output->depth < m_zbuffer[pos.y][pos.x])
            {
                m_zbuffer[pos.y][pos.x] = output->depth;
                // TODO：输出缓冲区,这里为了方便直接输出
                DrawPixel(pos.x,pos.y,RGB(color.x,color.y,color.z));
            }
        }

    }
}

void Renderer::DrawPixel(int x, int y, COLORREF color)
{
    SetPixel(m_hdc, x, y, color);
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
            if (!PointInTriangleFastCheck(x0, y0, x1, y1, x2, y2, i, j))
                continue;

            Vector3f baryCentric = CalculateBarycentric(x0, y0, x1, y1, x2, y2, i, j);

            BYTE r = static_cast<BYTE>(r0 * baryCentric.x + r1 * baryCentric.y + r2 * baryCentric.z);
            BYTE g = static_cast<BYTE>(g0 * baryCentric.x + g1 * baryCentric.y + g2 * baryCentric.z);
            BYTE b = static_cast<BYTE>(b0 * baryCentric.x + b1 * baryCentric.y + b2 * baryCentric.z);

            COLORREF color = RGB(r, g, b);
            DrawPixel(i, j, color);
        }
    }
}

bool Renderer::IsTriangleOutsideFrustum(const Vector4f &v0, const Vector4f &v1, const Vector4f &v2)
{
    // 检查是否所有顶点都在某个裁剪平面外
    auto isOutside = [](float x, float y, float z)
    {
        return (x < -1.0f && y < -1.0f && z < -1.0f) ||
               (x > 1.0f && y > 1.0f && z > 1.0f);
    };

    return isOutside(v0.x, v0.y, v0.z) &&
           isOutside(v1.x, v1.y, v1.z) &&
           isOutside(v2.x, v2.y, v2.z);
}

bool Renderer::IsTriangleInsideFrustum(const Vector4f &v0, const Vector4f &v1, const Vector4f &v2)
{
    auto isInside = [](float x, float y, float z)
    {
        return (x >= -1.0f && x <= 1.0f) &&
               (y >= -1.0f && y <= 1.0f) &&
               (z >= -1.0f && z <= 1.0f);
    };

    return isInside(v0.x, v0.y, v0.z) &&
           isInside(v1.x, v1.y, v1.z) &&
           isInside(v2.x, v2.y, v2.z);
}

void Renderer::ClipPolygon(vector<Vector4f> &polygon)
{
    // 定义裁剪平面（x=-1, x=1, y=-1, y=1, z=-1, z=1）
    const vector<pair<Vector4f, Vector4f>> clipPlanes = {
        {Vector4f(1.0f, 0.0f, 0.0f, 1.0f), Vector4f(-1.0f, 0.0f, 0.0f, 1.0f)}, // x = -1
        {Vector4f(-1.0f, 0.0f, 0.0f, 1.0f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f)}, // x = 1
        {Vector4f(0.0f, 1.0f, 0.0f, 1.0f), Vector4f(0.0f, -1.0f, 0.0f, 1.0f)}, // y = -1
        {Vector4f(0.0f, -1.0f, 0.0f, 1.0f), Vector4f(0.0f, 1.0f, 0.0f, 1.0f)}, // y = 1
        {Vector4f(0.0f, 0.0f, 1.0f, 1.0f), Vector4f(0.0f, 0.0f, -1.0f, 1.0f)}, // z = -1
        {Vector4f(0.0f, 0.0f, -1.0f, 1.0f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f)}  // z = 1
    };

    for (const auto &plane : clipPlanes)
    {
        vector<Vector4f> output;
        const Vector4f &planeNormal = plane.first;
        const Vector4f &planePoint = plane.second;

        for (size_t i = 0; i < polygon.size(); i++)
        {
            const Vector4f &current = polygon[i];
            const Vector4f &next = polygon[(i + 1) % polygon.size()];

            float currentSide = (current - planePoint).Dot(planeNormal);
            float nextSide = (next - planePoint).Dot(planeNormal);

            if (currentSide >= 0) // 当前点在平面内或平面上
                output.push_back(current);

            if (currentSide * nextSide < 0) // 线段与平面相交
            {
                float t = currentSide / (currentSide - nextSide);
                Vector4f intersection = current.Lerp(next, t);
                output.push_back(intersection);
            }
        }

        polygon = output;
        if (polygon.empty())
            break;
    }
}

Vector3f inline Renderer::CalculateBarycentric(int x0, int y0, int x1, int y1, int x2, int y2, int xp, int yp) noexcept
{
    // Vector2i v0(x1 - x0, y1 - y0);
    // Vector2i v1(x2 - x0, y2 - y0);
    // Vector2i vp(xp - x0, yp - y0);

    // // 解线性方程
    // int64_t  d00 = v0.Dot(v0);
    // int64_t  d01 = v0.Dot(v1);
    // int64_t  d10 = v1.Dot(v0);
    // int64_t  d11 = v1.Dot(v1);
    // int64_t  c0 = vp.Dot(v0);
    // int64_t  c1 = vp.Dot(v1);

    // float det = d11 * d00 - d01 * d10;
    // float det1 = d11 * c0 - d01 * c1;
    // float det2 = c1 * d00 - c0 * d10;

    // // 无解
    // if (std::abs(det) < 1e-6f)
    // {
    //     return Vector3f(-1, -1, -1); // 返回无效坐标
    // }

    // // 克拉默法则
    // float beta = det1 / det;
    // float gamma = det2 / det;
    // float alpha = 1 - beta - gamma;
    // return Vector3f(alpha, beta, gamma);

    // 优化版本
    float detT = static_cast<float>((y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2));
    if (fabs(detT) < 1e-6f)
        return Vector3f(-1, -1, -1);

    float alpha = ((y1 - y2) * (xp - x2) + (x2 - x1) * (yp - y2)) / detT;
    float beta = ((y2 - y0) * (xp - x2) + (x0 - x2) * (yp - y2)) / detT;
    float gamma = 1.0f - alpha - beta;

    return Vector3f(alpha, beta, gamma);
}

bool inline Renderer::PointInTriangleFastCheck(int x0, int y0, int x1, int y1, int x2, int y2, int xp, int yp) noexcept
{
    // 使用整数运算避免浮点数开销
    int64_t d1 = (xp - x0) * (y1 - y0) - (yp - y0) * (x1 - x0);
    int64_t d2 = (xp - x1) * (y2 - y1) - (yp - y1) * (x2 - x1);
    int64_t d3 = (xp - x2) * (y0 - y2) - (yp - y2) * (x0 - x2);

    // 检查符号是否一致（允许在边上）
    return ((d1 >= 0 && d2 >= 0 && d3 >= 0) ||
            (d1 <= 0 && d2 <= 0 && d3 <= 0));
}
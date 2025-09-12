#pragma once
#include "Math/Vector.h"
#include <memory>

using namespace std;

// 顶点着色器输出基类,着色器可以继承该类
class BaseVertexOutput
{
public:
    // 经过变换后的投影空间坐标
    Vector3f vPos;
    // 颜色
    Vector3i vColor;
};

class BasePixelInput
{
public:
    // 屏幕空间坐标
    Vector2i iPos;
    // 颜色
    Vector3i iColor;
};

// 像素着色器输出基类
class BasePixelOutput
{
public:
    // 屏幕空间坐标
    Vector2i oPos;
    // 颜色
    Vector3i oColor;
};

// 顶点着色器基类
class VertexShader
{
public:
    virtual ~VertexShader() = default;
    virtual shared_ptr<BaseVertexOutput> Process(const Vertex &vertexList) = 0;
};

// 像素着色器基类
class PixelShader
{
public:
    virtual ~PixelShader() = default;
    virtual shared_ptr<BasePixelOutput> Process(const BasePixelInput &vertexList) = 0;
};
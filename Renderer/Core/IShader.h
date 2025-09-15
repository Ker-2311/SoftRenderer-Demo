#pragma once
#include "Math/Vector.h"
#include <memory>
#include <unordered_map>

using namespace std;

// 可用的顶点属性类型
enum class AttributeType
{
    Color,
    Normal,
    UV,
};

// 顶点着色器输出基类,着色器可以继承该类
class BaseVertexOutput
{
public:
    // 经过变换后的投影空间坐标
    Vector4f oPos;
    // 顶点属性
    unordered_map<AttributeType, Vector4f> attributes;
};

class PixelInput
{
public:
    // 屏幕空间坐标
    Vector2i iPos;
    // 像素属性
    unordered_map<AttributeType, Vector4f> attributes;
    // 深度值
    float depth;
};

// 像素着色器输出基类
class BasePixelOutput
{
public:
    // 屏幕空间坐标
    Vector2i oPos;
    // 像素属性
    unordered_map<AttributeType, Vector4f> attributes;
    // 深度值
    float depth;
};

// 顶点着色器基类
class VertexShader
{
public:
    virtual ~VertexShader() = default;
    // 必须要做的事：将坐标扩展为(x,y,z,1)，并且乘MVP矩阵
    virtual shared_ptr<BaseVertexOutput> Process(const Vertex &vertex) = 0;
};

// 像素着色器基类
class PixelShader
{
public:
    virtual ~PixelShader() = default;
    virtual shared_ptr<BasePixelOutput> Process(const PixelInput &pixel) = 0;
};
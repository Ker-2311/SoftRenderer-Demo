#pragma once
#include "Core/IShader.h"
#include "Core/Vertex.h"


class DiffuseVS : public VertexShader
{
    shared_ptr<BaseVertexOutput> Process(const Vertex &vertex)
    {
        shared_ptr<BaseVertexOutput> output = make_shared<BaseVertexOutput>();
        Vector3f vPos = vertex.pos;
        Vector3f vNorm = vertex.normal;
        output->oPos = Vector4f(vPos.x, vPos.y, vPos.z, 1) * MVPMatrix;
        output->attributes[AttributeType::Color] = vertex.color;
        output->attributes[AttributeType::Normal] = Vector4f(vNorm.x,vNorm.y,vNorm.z,1) * M;
        output->attributes[AttributeType::UV] = vertex.uv;
        // 传递世界空间位置，用于光照计算
        Vector4f worldPos = Vector4f(vPos.x, vPos.y, vPos.z, 1) * M;
        output->attributes[AttributeType::WorldPos] = worldPos;

        return output;
    }
};

class DiffusePS : public PixelShader
{
    shared_ptr<BasePixelOutput> Process(const PixelInput &pixel)
    {
        shared_ptr<BasePixelOutput> output = make_shared<BasePixelOutput>();
        output->oPos = pixel.iPos;
        output->attributes = pixel.attributes;
        output->depth = pixel.depth;

        // 获取插值后的属性
        Vector4f color = output->attributes[AttributeType::Color];
        Vector4f normal = output->attributes[AttributeType::Normal];
        Vector4f worldPos = output->attributes[AttributeType::WorldPos];
        
        // 归一化法线
        Vector3f N = Vector3f(normal.x, normal.y, normal.z).Normalized();
        
        // 假设光源方向（从表面指向光源）
        Vector3f L = Vector3f(1.0f, 1.0f, 0.5f).Normalized(); // 示例光源方向
        
        // 计算漫反射强度（Lambert光照模型）
        float diffuse = max(0.0f, N.Dot(L));
        
        // 基础颜色（可以从纹理采样，这里使用顶点颜色）
        // Vector3f baseColor = Vector3f(color.x, color.y, color.z);
        Vector3f baseColor = Vector3f(1, 1, 1);
        
        // 光照颜色（白色光）
        Vector3f lightColor = Vector3f(1.0f, 1.0f, 1.0f);
        
        // 环境光强度
        float ambient = 0.1f;
        
        // 最终颜色 = 环境光 + 漫反射
        Vector3f finalColor = baseColor * (lightColor * diffuse + ambient);
        
        // 设置输出颜色
        output->attributes[AttributeType::Color] = Vector4f(finalColor.x, finalColor.y, finalColor.z, color.w);
        
        return output;
    }
};
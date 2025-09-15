#pragma once
#include "Core/IShader.h"
#include <Vertex.h>

class DefaultVS : public VertexShader
{
    shared_ptr<BaseVertexOutput> Process(const Vertex &vertex)
    {
        shared_ptr<BaseVertexOutput> output = make_shared<BaseVertexOutput>();
        Vector3f vPos = vertex.pos;
        output->oPos = Vector4f(vPos.x, vPos.y, vPos.z, 1) * MVPMatrix;
        output->attributes[AttributeType::Color] = vertex.color;
        output->attributes[AttributeType::Normal] = vertex.normal;
        output->attributes[AttributeType::UV] = vertex.uv;

        return output;
    }
};

class DefaultPS : public PixelShader
{
    shared_ptr<BasePixelOutput> Process(const PixelInput &pixel)
    {
        shared_ptr<BasePixelOutput> output = make_shared<BasePixelOutput>();
        output->oPos = pixel.iPos;
        output->attributes = pixel.attributes;
        output->depth = pixel.depth;
        return output;
    }
};
#pragma once
#include <vector>
#include <Vertex.h>
#include <Transform.h>
#include "IShader.h"

using namespace std;


class Mesh
{
public:
    Transform transform;
    vector<Vertex> vertexBuffer;
    vector<Vector3i> indexBuffer;
    shared_ptr<VertexShader> vs;
    shared_ptr<PixelShader> ps;
    
private:

};
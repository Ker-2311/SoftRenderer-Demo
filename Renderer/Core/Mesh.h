#pragma once
#include <vector>
#include <Vertex.h>
#include <Transform.h>

using namespace std;


class Mesh
{
public:
    Transform transform;
    vector<Vertex> vertexBuffer;
    vector<Vector3i> indexBuffer;
    
private:

};
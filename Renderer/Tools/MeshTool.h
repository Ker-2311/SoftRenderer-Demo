#include "Core/Renderer.h"
#include <iostream>
#include "Core/Mesh.h"
#include "Shaders/DiffuseShader.h"

class MeshTool
{
public:
    static Mesh *CreateTetrahedronMesh(Vector3f pos = Vector3f(0, 0, 5), Vector3f rot = Vector3f(0, 0, 0));
    static Mesh *CreateCubeMesh(Vector3f pos = Vector3f(0, 0, 5), Vector3f rot = Vector3f(0, 0, 0));
    static Mesh *CreateIcosahedronMesh(Vector3f pos = Vector3f(0, 0, 5), Vector3f rot = Vector3f(0, 0, 0));
};
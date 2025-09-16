#pragma once
#include "Math/Vector.h"

class Vertex
{
public:
    Vector3f pos = Vector3f(0, 0, 0);
    Vector3f color = Vector3f(0, 0, 0);
    Vector3f normal = Vector3f(0, 0, 0);
    Vector2f uv = Vector2f(0, 0);

public:
    Vertex(){}
    Vertex(Vector3f p) : pos(p) {}
    Vertex(Vector3f p, Vector3f c) : pos(p), color(c) {}
    Vertex(Vector3f p, Vector3f c, Vector3f normal) : pos(p), color(c), normal(normal) {}

    COLORREF GetColorWithColorRef() { return RGB(color.x, color.y, color.z); }
};
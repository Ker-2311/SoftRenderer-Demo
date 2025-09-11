#pragma once
#include "Math/Vector.h"

class Vertex
{
private:
    Vector3f m_pos = Vector3f(0,0,0);
    Vector3i m_color = Vector3i(0,0,0);
    Vector3f m_normal = Vector3f(0,0,0);
    Vector2f m_uv = Vector2f(0,0);

public:
    Vertex();
    Vertex(Vector3f pos) : m_pos(pos) {}
    Vertex(Vector3f pos, Vector3i color) : m_pos(pos), m_color(color) {}
    Vertex(Vector3f pos, Vector3i color, Vector3f normal) : m_pos(pos), m_color(color), m_normal(normal) {}

    COLORREF GetColor() { return RGB(m_color.x, m_color.y, m_color.z); }
};
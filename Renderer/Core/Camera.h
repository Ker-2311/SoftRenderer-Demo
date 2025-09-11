#pragma once
#include "Transform.h"
class Camera
{
public:
// 	Vector3 position;
// 	Vector3 rotation;
	Transform transform;
	Matrix4x4f v, p;
public:
	Camera(Transform t) : transform(t){}
	Camera() {}
	Matrix4x4f Perspective(float fov, float aspect, float nearPanel, float farPanel);
	Matrix4x4f Perspective(float l, float r, float n, float f, float t, float b);
	Matrix4x4f Orthographic(float l, float r, float n, float f, float t, float b);
};

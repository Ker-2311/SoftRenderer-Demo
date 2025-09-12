#pragma once
#include "Transform.h"
class Camera
{
public:
	Transform transform;

public:
	Camera(Transform t) : transform(t) {}

	Matrix4x4f GetViewMatrix()
	{
		Vector3f position = transform.position;
		Matrix4x4f sInv = {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        position.x, position.y, position.z, 1};
		Matrix4x4f rT = transform.GetRotationMatrix().Transpose();
		return sInv*rT;
	}

	Matrix4x4f Perspective(float fov, float aspect, float nearPanel, float farPanel);
	Matrix4x4f Perspective(float l, float r, float n, float f, float t, float b);
	Matrix4x4f Orthographic(float l, float r, float n, float f, float t, float b);
};

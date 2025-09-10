#pragma once
#include "Math/Matrix.h"
class Transform
{
public:
	Vector3f position;
	Vector3f rotation;
	Vector3f scale;
	Matrix4x4f objectToWorld;
	Matrix4x4f Translate(const Vector3f &v);
	Matrix4x4f RotateX(float angle);
	Matrix4x4f RotateY(float angle);
	Matrix4x4f RotateZ(float angle);
	Matrix4x4f Rotate(const Vector3f& rotAngle);
	Matrix4x4f Scale(const Vector3f &s);
	Transform(Vector3f pos, Vector3f rot, Vector3f s) :position(pos), rotation(rot), scale(s) {}
	Transform() { objectToWorld.Identity(); }
};

#pragma once
#include "Transform.h"
class Camera
{
public:
	Transform transform;
	float fov;		 // 垂直视野角度（弧度）
	float aspect;	 // 宽高比（宽度/高度）
	float nearPlane; // 近裁剪面
	float farPlane;	 // 远裁剪面

public:
	Camera()
		: transform(Transform()),		// 默认位置在原点，无旋转
		  fov(DegreesToRadians(60.0f)), // 默认60度视野
		  aspect(16.0f / 9.0f),			// 默认16:9宽高比
		  nearPlane(0.1f),				// 默认近裁剪面0.1
		  farPlane(100.0f)				// 默认远裁剪面100
	{
	}

	Camera(const Transform &t, float verticalFovDegrees = 60.0f, float aspectRatio = 16.0f / 9.0f, float nearClip = 0.1f, float farClip = 100.0f)
		: transform(t),
		  fov(DegreesToRadians(verticalFovDegrees)),
		  aspect(aspectRatio),
		  nearPlane(nearClip),
		  farPlane(farClip)
	{
	}

	Matrix4x4f GetViewMatrix()
	{
		Vector3f position = transform.position;
		Matrix4x4f sInv = {1, 0, 0, 0,
						   0, 1, 0, 0,
						   0, 0, 1, 0,
						   -position.x, -position.y, -position.z, 1};
		Matrix4x4f rT = transform.GetRotationMatrix().Transpose();
		return sInv * rT;
	}

	// 透视投影矩阵
	Matrix4x4f Perspective() const
	{
		float tanHalfFov = tan(fov / 2.0f);
		float range = farPlane - nearPlane;

		// 将坐标映射到[-1,1]之间
		Matrix4x4f proj;
		proj.data[0][0] = 1.0f / (aspect * tanHalfFov);
		proj.data[1][1] = 1.0f / tanHalfFov;
		proj.data[3][2] = (farPlane + nearPlane) / range;      
		proj.data[3][3] = 2.0f * farPlane * nearPlane / range; 
		proj.data[2][3] = -1.0f;                     // 透视除法标记位-1表示右手坐标系
		
		// 其余位置清零
		proj.data[0][1] = proj.data[0][2] = proj.data[0][3] = 0.0f;
		proj.data[1][0] = proj.data[1][2] = proj.data[1][3] = 0.0f;
		proj.data[2][0] = proj.data[2][1] = proj.data[2][2] = 0.0f;
		proj.data[3][0] = proj.data[3][1] = 0.0f;

		return proj;
	}

	// 正交投影矩阵
	static Matrix4x4f Orthographic(float left, float right, float bottom, float top, float near, float far)
	{
		Matrix4x4f proj;
		proj.data[0][0] = 2.0f / (right - left);
		proj.data[1][1] = 2.0f / (top - bottom);
		proj.data[2][2] = -2.0f / (far - near);
		proj.data[3][0] = -(right + left) / (right - left);
		proj.data[3][1] = -(top + bottom) / (top - bottom);
		proj.data[3][2] = -(far + near) / (far - near);
		proj.data[3][3] = 1.0f;

		return proj;
	}

private:
	// 角度转弧度
	static float DegreesToRadians(float degrees)
	{
		return degrees * (3.14159265358979323846f / 180.0f);
	}
};

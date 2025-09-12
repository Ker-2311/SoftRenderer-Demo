#pragma once
#include "Math/Matrix.h"
class Transform
{
public:
    Vector3f position = Vector3f(0, 0, 0);
    Vector3f rotation = Vector3f(0, 0, 0);
    Vector3f scale = Vector3f(1, 1, 1);

    void Translate(const Vector3f &v) { position = position + v; }
    void RotateX(float angle) { rotation = rotation + Vector3f(angle, 0, 0); }
    void RotateY(float angle) { rotation = rotation + Vector3f(0, angle, 0); }
    void RotateZ(float angle) { rotation = rotation + Vector3f(0, 0, angle); }
    void Rotate(const Vector3f &rotAngle) { rotation = rotation + rotAngle; }
    void Scale(const Vector3f &s) { scale = scale + s; }
    // 获取世界变换矩阵
    Matrix4x4f GetModelToWorldMatrix()
    {
        Matrix4x4f S = {scale.x, 0, 0, 0,
                        0, scale.y, 0, 0,
                        0, 0, scale.z, 0,
                        0, 0, 0, 1};
        Matrix4x4f R = CreateRotationMatrix(rotation);
        Matrix4x4f T = {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        position.x, position.y, position.z, 1};

        return S * R * T;
    }
    Matrix4x4f GetRotationMatrix() {return CreateRotationMatrix(rotation);}
    Transform(Vector3f pos, Vector3f rot, Vector3f s) : position(pos), rotation(rot), scale(s) {}
    Transform();

private:
    // 根据欧拉角创建旋转矩阵（XYZ 顺序）
    static Matrix4x4f CreateRotationMatrix(const Vector3f &eulerAngles)
    {
        float cx = cos(eulerAngles.x);
        float sx = sin(eulerAngles.x);
        float cy = cos(eulerAngles.y);
        float sy = sin(eulerAngles.y);
        float cz = cos(eulerAngles.z);
        float sz = sin(eulerAngles.z);

        // X 轴旋转矩阵
        Matrix4x4f Rx = {
            1, 0, 0, 0,
            0, cx, -sx, 0,
            0, sx, cx, 0,
            0, 0, 0, 1};

        // Y 轴旋转矩阵
        Matrix4x4f Ry = {
            cy, 0, sy, 0,
            0, 1, 0, 0,
            -sy, 0, cy, 0,
            0, 0, 0, 1};

        // Z 轴旋转矩阵
        Matrix4x4f Rz = {
            cz, -sz, 0, 0,
            sz, cz, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};

        // 组合旋转顺序：Z -> Y -> X
        return Rz * Ry * Rx;
    }
};

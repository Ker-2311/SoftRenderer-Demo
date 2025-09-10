#pragma once
#include <iterator>
#include <Math/Vector.h>

#include <cstddef>
#include <stdexcept>
#include <iostream>

template <typename T, size_t Row, size_t Col>
struct Matrix {
    T data[Row][Col];

    // ####### 通用运算符重载 #######
    operator T*() { return &data[0][0]; }
    operator const T*() const { return &data[0][0]; }

    // ####### 矩阵基本操作 #######
    // 单位矩阵（仅方阵有效）
    Matrix& Identity() {
        static_assert(Row == Col, "Identity matrix must be square");
        for (size_t i = 0; i < Row; i++) {
            for (size_t j = 0; j < Col; j++) {
                data[i][j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
            }
        }
        return *this;
    }

    // ####### 矩阵加减法 #######
    // 加法（同维度矩阵）
    Matrix operator+(const Matrix& other) const {
        if (Row != other.Row || Col != other.Col) {
            throw std::invalid_argument("Matrix addition requires same dimensions");
        }
        Matrix result;
        for (size_t i = 0; i < Row; i++) {
            for (size_t j = 0; j < Col; j++) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    // 减法（同维度矩阵）
    Matrix operator-(const Matrix& other) const {
        if (Row != other.Row || Col != other.Col) {
            throw std::invalid_argument("Matrix subtraction requires same dimensions");
        }
        Matrix result;
        for (size_t i = 0; i < Row; i++) {
            for (size_t j = 0; j < Col; j++) {
                result.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
        return result;
    }

    // ####### 矩阵乘法 #######
    // 矩阵乘法（A的列数 == B的行数）
    template <size_t OtherCol>
    Matrix<T, Row, OtherCol> operator*(const Matrix<T, Col, OtherCol>& other) const {
        Matrix<T, Row, OtherCol> result{};
        for (size_t i = 0; i < Row; i++) {
            for (size_t j = 0; j < OtherCol; j++) {
                for (size_t k = 0; k < Col; k++) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    // ####### 标量乘/除法 #######
    // 矩阵乘标量
    Matrix operator*(T scalar) const {
        Matrix result;
        for (size_t i = 0; i < Row; i++) {
            for (size_t j = 0; j < Col; j++) {
                result.data[i][j] = data[i][j] * scalar;
            }
        }
        return result;
    }

    // 矩阵除标量
    Matrix operator/(T scalar) const {
        if (scalar == 0) throw std::invalid_argument("Division by zero");
        Matrix result;
        for (size_t i = 0; i < Row; i++) {
            for (size_t j = 0; j < Col; j++) {
                result.data[i][j] = data[i][j] / scalar;
            }
        }
        return result;
    }

    // ####### 其他工具函数 #######
    // 打印矩阵
    void Print() const {
        for (size_t i = 0; i < Row; i++) {
            for (size_t j = 0; j < Col; j++) {
                std::cout << data[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
};

// 常用别名
using Matrix3x3f = Matrix<float, 3, 3>;
using Matrix4x4f = Matrix<float, 4, 4>;
#pragma once
#include <iterator>
#include "Matrix.h"
#include <algorithm>

template <typename T> struct Vector2Type;
template <typename T> struct Vector3Type;
template <typename T> struct Vector4Type;

// 定义通用Vector类型，使用CRTP模式
template <typename T, size_t N, typename Derived>
struct VectorTypeBase
{
	union
	{
		T data[N];
		struct
		{
			T x, y, z, w;
		};
	};

	VectorTypeBase() = default;

	// #######通用运算符重载#######
	Derived &operator=(const Derived &other)
	{
		if (this != &other)
		{
			for (size_t i = 0; i < N; ++i)
			{
				data[i] = other.data[i];
			}
		}
		return *this;
	}

	Derived operator+(const Derived &other) const
	{
		Derived res;
		for (size_t i = 0; i < N; ++i)
		{
			res.data[i] = data[i] + other.data[i];
		}
		return res;
	}

	Derived operator-(const Derived &other) const
	{
		Derived res;
		for (size_t i = 0; i < N; ++i)
		{
			res.data[i] = data[i] - other.data[i];
		}
		return res;
	}

	Derived operator*(const Derived &other) const
	{
		Derived res;
		for (size_t i = 0; i < N; ++i)
		{
			res.data[i] = data[i] * other.data[i];
		}
		return res;
	}

	Derived operator/(const Derived &other) const
	{
		Derived res;
		for (size_t i = 0; i < N; ++i)
		{
			if (other.data[i] == 0)
				throw std::runtime_error("Division by zero");
			res.data[i] = data[i] / other.data[i];
		}
		return res;
	}

	Derived operator+(const T &value) const
	{
		Derived res;
		for (size_t i = 0; i < N; ++i)
		{
			res.data[i] = data[i] + value;
		}
		return res;
	}

	Derived operator-(const T &value) const
	{
		Derived res;
		for (size_t i = 0; i < N; ++i)
		{
			res.data[i] = data[i] - value;
		}
		return res;
	}

	Derived operator*(const T &value) const
	{
		Derived res;
		for (size_t i = 0; i < N; ++i)
		{
			res.data[i] = data[i] * value;
		}
		return res;
	}

	Derived operator/(const T &value) const
	{
		if (value == 0)
			throw std::runtime_error("Division by zero");
		Derived res;
		for (size_t i = 0; i < N; ++i)
		{
			res.data[i] = data[i] / value;
		}
		return res;
	}

	// 向量乘矩阵
	template <size_t OtherCol>
	Derived operator*(const Matrix<T, N, OtherCol> &mat) const
	{
		static_assert(N == OtherCol, "Vector dimension must match matrix column count");
		Derived result;
		for (size_t i = 0; i < N; ++i)
		{
			result.data[i] = static_cast<T>(0);
			for (size_t j = 0; j < N; ++j)
			{
				result.data[i] += data[j] * mat.data[j][i]; // 注意矩阵访问顺序
			}
		}
		return result;
	}

	// #######通用向量操作#######
	inline T Sum() const
	{
		T res = 0;
		for (size_t i = 0; i < N; i++)
		{
			res += data[i];
		}
		return res;
	}

	inline T Dot(const Derived &v)
	{
		T res = 0;
		for (size_t i = 0; i < N; i++)
		{
			res += data[i] * v.data[i];
		}
		return res;
	}

	inline Derived Cross(const Derived &v)
	{
		static_assert(N == 3, "Cross product only defined for 3D vectors");

		if constexpr (N == 3)
		{
			return {
				data[1] * v.data[2] - data[2] * v.data[1],
				data[2] * v.data[0] - data[0] * v.data[2],
				data[0] * v.data[1] - data[1] * v.data[0]};
		}
		return Derived{};
	}

	inline Derived Normalize()
	{
		T len = std::sqrt(Dot(this));
		if (len == 0)
			throw std::runtime_error("Cannot normalize zero vector");

		Derived res;
		for (size_t i = 0; i < N; i++)
		{
			res.data[i] = data[i] / len;
		}
		return res;
	}

	inline Derived Lerp(const Derived &b, T t) const
	{
		t = std::clamp(t, static_cast<T>(0), static_cast<T>(1));
		return static_cast<const Derived &>(*this) + (b - static_cast<const Derived &>(*this)) * t;
	}
};

template <typename T>
struct Vector2Type : public VectorTypeBase<T, 2, Vector2Type<T>>
{
	using VectorTypeBase<T, 2, Vector2Type<T>>::x;
	using VectorTypeBase<T, 2, Vector2Type<T>>::y;
	using VectorTypeBase<T, 2, Vector2Type<T>>::data;

	Vector2Type() : VectorTypeBase<T, 2, Vector2Type<T>>() {}
	Vector2Type(const T &_x, const T &_y) : VectorTypeBase<T, 2, Vector2Type<T>>()
	{
		x = _x;
		y = _y;
	}
	Vector2Type(const Vector3Type<T> &v3) : VectorTypeBase<T, 2, Vector2Type<T>>()
	{
		x = v3.x;
		y = v3.y;
	}

	Vector2Type(const Vector4Type<T> &v4) : VectorTypeBase<T, 2, Vector2Type<T>>()
	{
		x = v4.x;
		y = v4.y;
	}

	Vector2Type &operator=(const Vector3Type<T> &v3)
	{
		x = v3.x;
		y = v3.y;
		return *this;
	}

	Vector2Type &operator=(const Vector4Type<T> &v4)
	{
		x = v4.x;
		y = v4.y;
		return *this;
	}
	// 使用基类的运算符重载
	using VectorTypeBase<T, 2, Vector2Type<T>>::operator=;
	using VectorTypeBase<T, 2, Vector2Type<T>>::operator+;
	using VectorTypeBase<T, 2, Vector2Type<T>>::operator-;
	using VectorTypeBase<T, 2, Vector2Type<T>>::operator*;
	using VectorTypeBase<T, 2, Vector2Type<T>>::operator/;
	// 通用向量操作
	using VectorTypeBase<T, 2, Vector2Type<T>>::Sum;
	using VectorTypeBase<T, 2, Vector2Type<T>>::Dot;
	using VectorTypeBase<T, 2, Vector2Type<T>>::Normalize;
};

template <typename T>
struct Vector3Type : public VectorTypeBase<T, 3, Vector3Type<T>>
{
	using VectorTypeBase<T, 3, Vector3Type<T>>::x;
	using VectorTypeBase<T, 3, Vector3Type<T>>::y;
	using VectorTypeBase<T, 3, Vector3Type<T>>::z;
	using VectorTypeBase<T, 3, Vector3Type<T>>::data;

	Vector3Type() : VectorTypeBase<T, 3, Vector3Type<T>>() {}
	Vector3Type(const T &_x, const T &_y, const T &_z) : VectorTypeBase<T, 3, Vector3Type<T>>()
	{
		x = _x;
		y = _y;
		z = _z;
	}
	Vector3Type(const Vector4Type<T> &v4) : VectorTypeBase<T, 3, Vector3Type<T>>()
	{
		this.data = v4.data;
		w = 0;
	}
	Vector3Type(const Vector2Type<T> &v2) : VectorTypeBase<T, 3, Vector3Type<T>>()
	{
		this.data = v2.data;
		z = 0;
		w = 0;
	}

	Vector3Type &operator=(const Vector2Type<T> &v2)
	{
		this.data = v2.data;
		z = 0;
		w = 0;
		return *this;
	}
	Vector3Type &operator=(const Vector4Type<T> &v4)
	{
		this.data = v4.data;
		w = 0;
		return *this;
	}
	// 使用基类的运算符重载
	using VectorTypeBase<T, 3, Vector3Type<T>>::operator=;
	using VectorTypeBase<T, 3, Vector3Type<T>>::operator+;
	using VectorTypeBase<T, 3, Vector3Type<T>>::operator-;
	using VectorTypeBase<T, 3, Vector3Type<T>>::operator*;
	using VectorTypeBase<T, 3, Vector3Type<T>>::operator/;
	// 通用向量操作
	using VectorTypeBase<T, 3, Vector3Type<T>>::Sum;
	using VectorTypeBase<T, 3, Vector3Type<T>>::Dot;
	using VectorTypeBase<T, 3, Vector3Type<T>>::Cross;
	using VectorTypeBase<T, 3, Vector3Type<T>>::Normalize;
};

template <typename T>
struct Vector4Type : public VectorTypeBase<T, 4, Vector4Type<T>>
{
	using VectorTypeBase<T, 4, Vector4Type<T>>::x;
	using VectorTypeBase<T, 4, Vector4Type<T>>::y;
	using VectorTypeBase<T, 4, Vector4Type<T>>::z;
	using VectorTypeBase<T, 4, Vector4Type<T>>::data;

	Vector4Type() : VectorTypeBase<T, 4, Vector4Type<T>>() {}
	Vector4Type(const T &_x, const T &_y, const T &_z, const T &_w) : VectorTypeBase<T, 4, Vector4Type<T>>()
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	Vector4Type(const Vector3Type<T> &v3) : VectorTypeBase<T, 4, Vector4Type<T>>()
	{
		this.data = v3.data;
		w = 0;
	}
	Vector4Type(const Vector2Type<T> &v2) : VectorTypeBase<T, 4, Vector4Type<T>>()
	{
		this.data = v2.data;
		z = 0;
		w = 0;
	}

	Vector4Type &operator=(const Vector2Type<T> &v2)
	{
		this.data = v2.data;
		z = 0;
		w = 0;
		return *this;
	}
	Vector4Type &operator=(const Vector3Type<T> &v3)
	{
		this.data = v3.data;
		w = 0;
		return *this;
	}
	// 使用基类的运算符重载
	using VectorTypeBase<T, 4, Vector4Type<T>>::operator=;
	using VectorTypeBase<T, 4, Vector4Type<T>>::operator+;
	using VectorTypeBase<T, 4, Vector4Type<T>>::operator-;
	using VectorTypeBase<T, 4, Vector4Type<T>>::operator*;
	using VectorTypeBase<T, 4, Vector4Type<T>>::operator/;
	// 通用向量操作
	using VectorTypeBase<T, 4, Vector4Type<T>>::Sum;
	using VectorTypeBase<T, 4, Vector4Type<T>>::Dot;
	using VectorTypeBase<T, 4, Vector4Type<T>>::Normalize;
	using VectorTypeBase<T, 4, Vector4Type<T>>::Lerp;
};

typedef Vector2Type<float> Vector2f;
typedef Vector2Type<int> Vector2i;
typedef Vector3Type<float> Vector3f;
typedef Vector3Type<int> Vector3i;
typedef Vector4Type<float> Vector4f;
typedef Vector4Type<int> Vector4i;
typedef Vector4Type<float> Quaternion;
typedef Vector4Type<float> Color;

// 定义一些全局的函数
namespace Vector
{
}

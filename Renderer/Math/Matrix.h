#pragma once
#include <iterator>
#include <Core/Math/Vector.h>

namespace EternalEngine::Math
{
	// 定义Matrix类型
	template <typename T, size_t Row, size_t Col>
	struct BaseMatrix
	{
		union 
		{
			T data[Row][Col];
		}

		// #######通用运算符重载#######
		operator T*() {
			return &data[0][0];
		}
		operator const T*() const {
			return static_cast<const T*>(&data[0][0]);
		};
		
	};
	
	typedef BaseMatrix<float,3,3> Matrix3x3f;
	typedef BaseMatrix<float,4,4> Matrix4x4f;

	// 定义矩阵操作函数
	namespace Matrix
	{

	}

} // namespace EternalEngine
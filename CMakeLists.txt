cmake_minimum_required(VERSION 3.10)
project(Eternal-Engine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(NOT MSVC)
    # 对于非 MSVC 编译器
    add_compile_options(-finput-charset=UTF-8)
    add_compile_options(-fexec-charset=UTF-8)
else()
    # 设置 C++ 源代码使用 UTF-8 编码
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

include_directories("${PROJECT_SOURCE_DIR}/Render")

include(CTest)

# 添加子目录
add_subdirectory(Renderer)
add_subdirectory(Tests)


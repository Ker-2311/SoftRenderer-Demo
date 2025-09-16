# 软渲染器Demo项目

## 1.项目介绍
这是一个非常简单的图形学学习项目,致力于实现在CPU上运行的软渲染器
本项目包括以下部分（以及建议按照以下顺序实现）：
1.数学运算库
2.WindowsAPI抽象层
3.一些基本的用于渲染的数据结构
4.MVP矩阵的运算(在行主序以及左手坐标系下)
5.通过Sutherland-Hodgman 算法进行图元裁切
6.背面剔除算法
7.透视矫正插值算法
8.通过重心坐标进行光栅化
9.深度测试


通过这个教程，你可以获得：

必备前置知识:
Windows API入门
图形学基础
C++基础知识

## 2.项目架构
```bash
Renderer
├── 数学库 (Math)
│   ├── 向量类 (Vector2/3/4)
│   ├── 矩阵类 (Matrix4x4)
│   ├── 几何工具 (几何变换、投影等)
├── 核心部分 (Core)
│    ├── 渲染器 (Renderer)
│    │   ├── 顶点处理 (Vertex Processing)
│    │   ├── 图元装配 (Primitive Assembly)
│    │   ├── 裁切 (Clip)
│    │   ├── 光栅化 (Rasterization)
│    │   ├── 片元处理 (Fragment Processing)
│    ├── 数据结构
│    │   ├── 相机 (Camera)
│    │   ├── 着色器基类 (IShader)
│    │   ├── 网格数据 (Mesh)
│    │   ├── 顶点 (Vertex)
│    │   ├── Transform (Transform)
├── 着色器系统 (Shaders)
│   ├── 默认着色器 (DefaultShader)
└── WindowsAPI抽象层 (Windows)
Tests
├── 数学库测试 (MathTest)
├── 渲染器测试 (RendererTest)
└── WindowsAPi测试 (WindowsTest)
```
## 3.项目演示

## 4.相关链接
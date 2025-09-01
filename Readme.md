# 软渲染器Demo项目

## 1.项目介绍

## 2.项目架构
```bash
Renderer
├── 数学库 (Math)
│   ├── 向量类 (Vector2/3/4)
│   ├── 矩阵类 (Matrix4x4)
│   ├── 几何工具 (几何变换、投影等)
├── 图形管线 (Pipeline)
│   ├── 顶点处理 (Vertex Processing)
│   ├── 图元装配 (Primitive Assembly)
│   ├── 光栅化 (Rasterization)
│   ├── 片元处理 (Fragment Processing)
├── 数据结构
│   ├── 帧缓冲区 (FrameBuffer)
│   ├── 深度缓冲区 (DepthBuffer)
│   ├── 网格数据 (Mesh)
│   ├── 纹理 (Texture)
├── 着色器系统 (Shader System)
│   ├── 顶点着色器 (Vertex Shader)
│   ├── 片元着色器 (Fragment Shader)
└── IO接口
    ├── 模型加载 (obj等)
    ├── 纹理读取 (BMP/PNG等)
    ├── 图像导出 (BMP/PNG等)
    └── 实时显示
```
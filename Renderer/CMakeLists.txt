set(CorePath ${PROJECT_SOURCE_DIR}/Renderer)

add_library(EternalRenderer 
${CorePath}/Windows/WindowsPlatform.cpp
${CorePath}/Core/Renderer.cpp
${CorePath}/Core/Mesh.cpp
${CorePath}/Tools/MeshTool.cpp
)
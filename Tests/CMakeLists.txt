add_executable(TestWindows WIN32 WindowsTest.cpp)
target_link_libraries(TestWindows user32 EternalRenderer)
add_test(NAME TestWindows COMMAND TestWindows)

add_executable(RendererTest WIN32 RendererTest.cpp)
target_link_libraries(RendererTest user32 EternalRenderer)
add_test(NAME RendererTest COMMAND RendererTest)
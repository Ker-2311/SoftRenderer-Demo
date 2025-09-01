add_executable(TestWindows WIN32 WindowsTest.cpp)
target_link_libraries(TestWindows user32 EternalRenderer)
add_test(NAME TestWindows COMMAND TestWindows)
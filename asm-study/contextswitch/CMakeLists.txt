#CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(AssemblyStudy)

#最適化無効
set(CMAKE_CXX_FLAGS "-O0")

add_executable(simple_contextswitch simple_contextswitch.cpp)

#ctest有効
enable_testing()
add_test(NAME contextswitch_test COMMAND simple_contextswitch)
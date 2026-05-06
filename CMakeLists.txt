#CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(UserLevelThread C CXX ASM)

#C++23の宣言
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#最適化無効
set(CMAKE_CXX_FLAGS "-O0")

add_subdirectory(src)

#ctest有効
enable_testing()
add_subdirectory(test)
add_subdirectory(examples)
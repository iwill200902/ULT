#CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(AssemblyStudy)
project(AssemblyStudy C CXX ASM)

#最適化無効
set(CMAKE_CXX_FLAGS "-O0")

add_library(contextswitch STATIC  contextswitch/contextswitch.s)
target_include_directories(contextswitch PUBLIC contextswitch)

add_executable(simple_contextswitch simple_contextswitch.cpp)
target_link_libraries(simple_contextswitch PRIVATE contextswitch)

#ctest有効
enable_testing()
add_test(
    NAME contextswitch_test 
    COMMAND simple_contextswitch
)
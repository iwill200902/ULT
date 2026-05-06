add_executable(simple_task simple_task.cpp)
target_compile_options(simple_task PRIVATE -Wall -Wextra)
target_link_libraries(simple_task PRIVATE ult_lib)
add_test(NAME SimpleTask COMMAND simple_task)
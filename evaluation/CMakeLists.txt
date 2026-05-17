add_executable(benchmark_ult benchmark_ult.cpp)
target_compile_options(benchmark_ult PRIVATE -Wall -Wextra)
target_link_libraries(benchmark_ult PRIVATE ult_lib)

add_executable(benchmark_klt benchmark_klt.cpp)
target_compile_options(benchmark_klt PRIVATE -Wall -Wextra)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(benchmark_klt PRIVATE Threads::Threads)
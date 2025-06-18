# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(MyHelloLibrary)

# ライブラリを定義（STATIC = 静的ライブラリ）
add_library(hello STATIC hello/hello.cpp)

# ライブラリにヘッダディレクトリを公開（include pathとして）
target_include_directories(hello PUBLIC hello)

# 実行ファイル main を作る
add_executable(main main.cpp)

# main にライブラリ hello をリンクする
target_link_libraries(main PRIVATE hello)

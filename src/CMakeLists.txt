#静的ライブラリを作成
add_libraly(ult_lib STATIC
    contextswitch.s
    ult.cpp
)

target_include_directories(ult_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
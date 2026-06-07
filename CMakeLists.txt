cmake_minimum_required(VERSION 3.10)

project(KSCdraw)

# 设置全局 C++ 标准（要求至少 C++17）
set(CMAKE_CXX_STANDARD 11)
# 要求必须使用这个标准，不可降级
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 可选：不要编译器自己的扩展（保持标准一致）
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(easyx STATIC IMPORTED)


set_target_properties(easyx PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/third_party/easyx/lib/libeasyx.a"
    # 同时指定头文件目录，让链接 easyx 的目标自动找到 easyx.h
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/third_party/easyx/include"
)

add_executable(KSCdraw master.c)

target_compile_options(KSCdraw PRIVATE -Wall -Wextra)

target_compile_definitions(KSCdraw PRIVATE __USE_AUTO__=1 __USE_PC__=1)

add_subdirectory(src)

target_link_libraries(KSCdraw PRIVATE KSCDRAW_SRC easyx)

# target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
#     # Add user defined include paths
#     KSCdraw/inc
#     KSCdraw/three_party/easyx/include
# )

# target_compile_definitions(KSCdraw PRIVATE __USE_PC__=1)
set_source_files_properties(KSCdisplay.c PROPERTIES LANGUAGE CXX)


cmake_minimum_required(VERSION 3.20)
project(KSCdraw VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_source_files_properties(src/KSCdisplay.c PROPERTIES LANGUAGE CXX)

set(KSCDRAW_SOURCES
    master.c
    src/application.c
    src/cmd.c
    src/KSCdisplay.c
    src/KSCdraw.c
    src/KSCfont.c
    src/KSCimg.c
    src/txt.c
    src/W25Q64.c
    littlefs/lfs.c
    littlefs/lfs_config.c
    littlefs/lfs_util.c
)

find_library(EASYX_LIB easyx
    HINTS ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES}
)
if(NOT EASYX_LIB)
    message(FATAL_ERROR "EasyX library not found. "
        "Set CMAKE_PREFIX_PATH or EASYX_LIB to the EasyX library path.")
endif()

add_executable(master ${KSCDRAW_SOURCES})

target_include_directories(master PRIVATE .)
target_link_libraries(master PRIVATE ${EASYX_LIB})
target_compile_options(master PRIVATE -Wall)

set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

add_custom_target(run
    COMMAND master
    DEPENDS master
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running master.exe..."
)

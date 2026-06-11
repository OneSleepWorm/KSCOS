file(GLOB_RECURSE KSCOS_src KSCOS/src/*.c KSCOS/third_party/stm32/src/*.c)
# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${KSCOS_src}

)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    KSCOS/inc
    KSCOS/third_party/stm32/inc
)

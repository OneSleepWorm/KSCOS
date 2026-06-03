file(GLOB_RECURSE KSCdraw_src KSCdraw/src/*.c KSCdraw/three_party/stm32/*.c)
# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${KSCdraw_src}
    
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    KSCdraw/inc
    KSCdraw/three_party/stm32
)

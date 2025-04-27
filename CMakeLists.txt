cmake_minimum_required(VERSION 3.10)
project(Echo2D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O3")

# Define target output directories in rootdir/target
set(TARGET_DIR "${CMAKE_SOURCE_DIR}/target")
set(BIN_DIR "${TARGET_DIR}/bin")
set(LIB_DIR "${TARGET_DIR}/lib")
set(INCLUDE_DIR "${TARGET_DIR}/include")

# Find dependencies
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLM REQUIRED)

# Collect all source files
add_library(Echo2D SHARED 
   src/engine/InputHandler.cpp
   src/engine/WindowHandler.cpp 
   src/engine/Application.cpp
   src/engine/Texture.cpp
   src/engine/Renderer.cpp
   src/external/stb.cpp
   src/external/glad.c
   src/external/imgui.cpp
   src/external/imgui_draw.cpp
   src/external/imgui_demo.cpp
   src/external/imgui_impl_glfw.cpp
   src/external/imgui_impl_opengl3.cpp
   src/external/imgui_tables.cpp
   src/external/imgui_widgets.cpp
   src/external/easylogging++.cpp
   src/utils/ShaderUtils.cpp
)

# Include directories
target_include_directories(Echo2D PUBLIC include ${GLM_INCLUDE_DIRS})

# Platform-specific linking
if (WIN32)
    target_link_libraries(Echo2D PRIVATE glfw opengl32)
elseif (APPLE)
    target_link_libraries(Echo2D PRIVATE glfw "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
elseif (UNIX)
    target_link_libraries(Echo2D PRIVATE glfw GL)
endif()

# Set output directories to rootdir/target
set_target_properties(Echo2D PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR}
)

# Copy headers to target/include while maintaining directory structure
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR}/include "include/*.h")
foreach(HEADER ${HEADERS})
    get_filename_component(HEADER_PATH ${HEADER} DIRECTORY)
    add_custom_command(
        OUTPUT ${INCLUDE_DIR}/${HEADER}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${INCLUDE_DIR}/${HEADER_PATH}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/include/${HEADER} ${INCLUDE_DIR}/${HEADER}
        DEPENDS ${CMAKE_SOURCE_DIR}/include/${HEADER}
    )
    list(APPEND HEADER_TARGETS ${INCLUDE_DIR}/${HEADER})
endforeach()

add_custom_target(copy_headers DEPENDS ${HEADER_TARGETS})

# Ensure the library depends on the header copy process
add_dependencies(Echo2D copy_headers)


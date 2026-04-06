cmake_minimum_required(VERSION 4.0)


# Set the project name and version
project(PlatformerMapEditor VERSION 0.1)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Platformer/lib/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Platformer/lib/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Platformer/bin/$<CONFIG>)


# Add all subdirectories
file(GLOB PlatformerMapEditor_src CONFIGURE_DEPENDS "src/*.cpp")
add_library(PlatformerMapEditorLib ${PlatformerMapEditor_src})
add_executable(PlatformerMapEditor src/main.cpp)
set_target_properties(PlatformerMapEditor PROPERTIES
WIN32_EXECUTABLE $<CONFIG:Release>) # Sets the WIN32_EXECUTABLE property ONLY on release
target_include_directories(PlatformerMapEditorLib INTERFACE "src")
target_link_libraries(PlatformerMapEditorLib PUBLIC PlatformerLib)
target_link_libraries(PlatformerMapEditor
PRIVATE
PlatformerMapEditorLib
)

# Statically link gcc into the executable, so anyone with the executable can play without downloading gcc
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(PlatformerMapEditor PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()
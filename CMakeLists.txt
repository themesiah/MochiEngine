cmake_minimum_required(VERSION 3.10)


# Set the project name and version
project(SpaceShooter VERSION 0.1)

enable_testing()
# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)

# Specify the C++ standard
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_23)

# Set some precompiling definitions depending on target
target_compile_definitions(compiler_flags INTERFACE "DEBUG")

# Link common use libraries
add_subdirectory(Includes/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(Includes/SDL3_image EXCLUDE_FROM_ALL)
target_link_libraries(compiler_flags
                      INTERFACE
                      SDL3_image::SDL3_image
                      SDL3::SDL3
)


#include_directories(
#    "${PROJECT_SOURCE_DIR}/Some/Include/Folder"
#)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-Wno-unused-variable;-Wno-unused-parameter>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# Add all subdirectories
add_subdirectory("src/Engine")
add_subdirectory("src/Test")
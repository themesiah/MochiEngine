cmake_minimum_required(VERSION 4.0)

project(MochiEngineSamples VERSION 0.1)

option(ENABLE_TESTS "Enable tests for all projects" OFF)
option(USE_FMOD "Uses FMOD instead of the default SDL3_Mixer audio backend" OFF)

if(ENABLE_TESTS)
enable_testing()
endif()

if(USE_FMOD)
message("Using FMOD as audio backend")
else()
message("Using SDL3 as audio backend")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/CMake/Utils.cmake)

# Samples
add_subdirectory("Engine")
add_subdirectory("Samples/SpaceShooter")
add_subdirectory("Samples/Platformer")
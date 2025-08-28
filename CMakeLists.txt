cmake_minimum_required(VERSION 3.10)


# Set the project name and version
project(SpaceShooter VERSION 0.1)

enable_testing()
# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)
set(LUA_VERSION "lua-5.4.8")

# Specify the C++ standard
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_23)

# Set some precompiling definitions depending on target
target_compile_definitions(compiler_flags INTERFACE $<$<CONFIG:Debug>:DEBUG>)

# Link common use libraries
set(SDLTTF_VENDORED ON)
add_subdirectory(Includes/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(Includes/SDL3_image EXCLUDE_FROM_ALL)
add_subdirectory(Includes/SDL3_ttf EXCLUDE_FROM_ALL)
add_subdirectory(Includes/FMOD EXCLUDE_FROM_ALL)
add_subdirectory(Includes/lua)
add_subdirectory(Includes/sol2)
include_directories("${CMAKE_SOURCE_DIR}/Includes/FMOD")
include_directories("${CMAKE_SOURCE_DIR}/Includes/Json")
target_link_libraries(compiler_flags
                      INTERFACE
                      SDL3_image::SDL3_image
                      SDL3::SDL3
                      SDL3_ttf::SDL3_ttf
                      fmod
                      fmodstudio
                      sol2
                      lua_static
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
add_subdirectory("src/Packer")

# Win32 mode (no console) or not
set(GAME_WIN32_FLAG "")

if(WIN32)
    # Only add the WIN32 flag in Release builds
    if(CMAKE_BUILD_TYPE STREQUAL "Release") # Single-config generators (Makefiles, Ninja)
        set(GAME_WIN32_FLAG "WIN32")
    elseif(CMAKE_CONFIGURATION_TYPES) # Multi-config generators (Visual Studio, Xcode)
        # For MSVC / multi-config, WIN32 is a target property so we set it after creation
        set(GAME_WIN32_FLAG "WIN32")
    endif()
endif()

add_executable(Game ${GAME_WIN32_FLAG} src/main.cpp)
target_link_libraries(Game
                      PUBLIC
                      Engine
)
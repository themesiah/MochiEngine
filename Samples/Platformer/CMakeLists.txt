cmake_minimum_required(VERSION 4.0)


# Set the project name and version
project(Platformer VERSION 0.1)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Platformer/lib/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Platformer/lib/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Platformer/bin/$<CONFIG>)


# Add all subdirectories
file(GLOB Platformer_src CONFIGURE_DEPENDS "src/*.cpp")
add_library(PlatformerLib ${Platformer_src})
add_executable(Platformer src/main.cpp)
set_target_properties(Platformer PROPERTIES
WIN32_EXECUTABLE $<CONFIG:Release>) # Sets the WIN32_EXECUTABLE property ONLY on release
target_include_directories(PlatformerLib INTERFACE "src")
target_link_libraries(PlatformerLib PUBLIC Engine)
target_link_libraries(Platformer
PRIVATE
PlatformerLib
)

# Statically link gcc into the executable, so anyone with the executable can play without downloading gcc
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(Platformer PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Platformer/bin/$<CONFIG>)
# Copy files after building
add_custom_command(
  TARGET Platformer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${PROJECT_SOURCE_DIR}/GameData"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data/Game"
  COMMENT "Copying data folder from ${PROJECT_SOURCE_DIR} TO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

copy_core(Platformer)

add_custom_command(TARGET Platformer POST_BUILD
    COMMAND $<$<CONFIG:Release>:$<TARGET_FILE:Packer>>
            $<$<CONFIG:Release>:-pack>
            $<$<CONFIG:Release>:$<TARGET_FILE_DIR:Platformer>/Data/Game>
            $<$<CONFIG:Release>:$<TARGET_FILE_DIR:Platformer>/Data/Game.pak>
    COMMAND $<$<CONFIG:Release>:$<TARGET_FILE:Packer>>
            $<$<CONFIG:Release>:-pack>
            $<$<CONFIG:Release>:$<TARGET_FILE_DIR:Platformer>/Data/Core>
            $<$<CONFIG:Release>:$<TARGET_FILE_DIR:Platformer>/Data/Core.pak>
    COMMAND $<$<CONFIG:Release>:${CMAKE_COMMAND}>
            $<$<CONFIG:Release>:-E>
            $<$<CONFIG:Release>:remove_directory>
            $<$<CONFIG:Release>:$<TARGET_FILE_DIR:Platformer>/Data/Core>
    COMMAND $<$<CONFIG:Release>:${CMAKE_COMMAND}>
            $<$<CONFIG:Release>:-E>
            $<$<CONFIG:Release>:remove_directory>
            $<$<CONFIG:Release>:$<TARGET_FILE_DIR:Platformer>/Data/Game>
    COMMENT "Packing Game assets (Release only)"
    VERBATIM
)

add_custom_target(copy_platformer_data_folder
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/GameData"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data/Game"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/../../Engine/Includes/FMOD/win/studio/lib/x64/fmodstudio.dll"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fmodstudio.dll"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/../../Engine/Includes/FMOD/win/core/lib/x64/fmod.dll"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fmod.dll"
        DEPENDS Platformer
    )

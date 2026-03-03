cmake_minimum_required(VERSION 4.0)


# Set the project name and version
project(SpaceShooter VERSION 0.1)

enable_testing()
# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SpaceShooter/lib/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SpaceShooter/lib/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SpaceShooter/bin/$<CONFIG>)


# Add all subdirectories
add_subdirectory("Tests")

file(GLOB SpaceShooter_src CONFIGURE_DEPENDS "src/*.cpp" "src/Bullets/*.cpp" "src/Enemies/*.cpp")
add_library(SpaceShooterLib ${SpaceShooter_src})
add_executable(SpaceShooter src/main.cpp)
set_target_properties(SpaceShooter PROPERTIES
WIN32_EXECUTABLE $<CONFIG:Release>) # Sets the WIN32_EXECUTABLE property ONLY on release
target_include_directories(SpaceShooterLib INTERFACE "src")
target_link_libraries(SpaceShooterLib PUBLIC Engine)
target_link_libraries(SpaceShooter
PRIVATE
SpaceShooterLib
)

# Statically link gcc into the executable, so anyone with the executable can play without downloading gcc
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(SpaceShooter PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SpaceShooter/bin/$<CONFIG>)
# Copy files after building
add_custom_command(
  TARGET SpaceShooter POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${PROJECT_SOURCE_DIR}/GameData"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data/Game"
  COMMENT "Copying data folder from ${PROJECT_SOURCE_DIR} TO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

copy_core(SpaceShooter)

add_custom_command(TARGET SpaceShooter POST_BUILD
    COMMAND $<$<CONFIG:Release>:$<TARGET_FILE:Packer>>
            $<$<CONFIG:Release>:-pack>
            $<$<CONFIG:Release>:$<TARGET_FILE_DIR:SpaceShooter>/Data/Game>
            $<$<CONFIG:Release>:$<TARGET_FILE_DIR:SpaceShooter>/Data/Game.pak>
    COMMAND $<$<CONFIG:Release>:$<TARGET_FILE:Packer>>
            $<$<CONFIG:Release>:-pack>
            $<$<CONFIG:Release>:$<TARGET_FILE_DIR:SpaceShooter>/Data/Core>
            $<$<CONFIG:Release>:$<TARGET_FILE_DIR:SpaceShooter>/Data/Core.pak>
    COMMAND $<$<CONFIG:Release>:${CMAKE_COMMAND}>
            $<$<CONFIG:Release>:-E>
            $<$<CONFIG:Release>:remove_directory>
            $<$<CONFIG:Release>:$<TARGET_FILE_DIR:SpaceShooter>/Data/Core>
    COMMAND $<$<CONFIG:Release>:${CMAKE_COMMAND}>
            $<$<CONFIG:Release>:-E>
            $<$<CONFIG:Release>:remove_directory>
            $<$<CONFIG:Release>:$<TARGET_FILE_DIR:SpaceShooter>/Data/Game>
    COMMENT "Packing Game assets (Release only)"
    VERBATIM
)

add_custom_target(copy_spaceshooter_data_folder
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/GameData"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data/Game"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/../../Engine/Includes/FMOD/win/studio/lib/x64/fmodstudio.dll"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fmodstudio.dll"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/../../Engine/Includes/FMOD/win/core/lib/x64/fmod.dll"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fmod.dll"
        DEPENDS SpaceShooter
    )

cmake_minimum_required(VERSION 4.0)


# Set the project name and version
project(SpaceShooter VERSION 0.1)



set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static-libgcc -static-libstdc++") # <-- Only for release!

enable_testing()
# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SpaceShooter/lib/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SpaceShooter/lib/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SpaceShooter/bin/$<CONFIG>)


# Add all subdirectories
add_subdirectory("../../Engine" "Engine")
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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SpaceShooter/bin/$<CONFIG>)
# Copy files after building
add_custom_command(
  TARGET SpaceShooter POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${PROJECT_SOURCE_DIR}/GameData"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data/Game"
  COMMENT "Copying data folder from ${PROJECT_SOURCE_DIR} TO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    # COMMAND ${CMAKE_COMMAND} -E copy
    #     "${PROJECT_SOURCE_DIR}/../../Engine/Includes/FMOD/win/studio/lib/x64/fmodstudio.dll"
    #     "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fmodstudio.dll"
    # COMMAND ${CMAKE_COMMAND} -E copy
    #     "${PROJECT_SOURCE_DIR}/../../Engine/Includes/FMOD/win/core/lib/x64/fmod.dll"
    #     "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fmod.dll"
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
cmake_minimum_required(VERSION 3.10)


# Set the project name and version
project(SpaceShooter VERSION 0.1)

enable_testing()
# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)


# Add all subdirectories
add_subdirectory("src/Core")
add_subdirectory("src/Engine")
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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)
# Copy files after building
add_custom_command(
  TARGET Game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${PROJECT_SOURCE_DIR}/Data"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data"
  COMMENT "Copying data folder from ${PROJECT_SOURCE_DIR} TO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)
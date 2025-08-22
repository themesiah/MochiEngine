# Get all .cpp files
file(GLOB Engine_src CONFIGURE_DEPENDS "Graphics/*.cpp" "*.cpp")

# Create the library
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)
add_executable(Engine ${Engine_src})

target_link_libraries(Engine PUBLIC
  compiler_flags
)

# Copy files after building
add_custom_command(
  TARGET Engine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${PROJECT_SOURCE_DIR}/Data"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data"
  COMMENT "Copying data folder from ${PROJECT_SOURCE_DIR} TO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)
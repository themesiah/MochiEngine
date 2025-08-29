# Get all .cpp files
file(GLOB Packer_src CONFIGURE_DEPENDS "*.cpp")

# Create the library
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/packer/$<CONFIG>)
add_executable(Packer ${Packer_src})

target_link_libraries(Packer PUBLIC
  Core
)

add_custom_command(
  TARGET Packer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${PROJECT_SOURCE_DIR}/Data"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data"
  COMMENT "Copying data folder from ${PROJECT_SOURCE_DIR} TO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)
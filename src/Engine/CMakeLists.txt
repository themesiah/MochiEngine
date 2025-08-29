# Get all .cpp files
file(GLOB Engine_src CONFIGURE_DEPENDS "*.cpp")

# Create the library
add_library(Engine ${Engine_src})

target_link_libraries(Engine PUBLIC
  Core
)
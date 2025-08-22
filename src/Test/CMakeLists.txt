# Get all .cpp files
file(GLOB Test_src CONFIGURE_DEPENDS "*.cpp" "doctest.h")

enable_testing()

message(${CMAKE_CURRENT_LIST_DIR})
include(${CMAKE_CURRENT_LIST_DIR}/scripts/doctest.cmake)

set(DOCTEST_INCLUDE_DIR Includes/doctest CACHE INTERNAL "Path to include folder for doctest")


# Create the library
add_executable(Testing ${Test_src})

target_link_libraries(Testing PUBLIC
  compiler_flags
)

doctest_discover_tests(Testing)
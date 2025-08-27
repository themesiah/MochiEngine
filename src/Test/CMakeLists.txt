project(Tests)
enable_testing()

include(${CMAKE_CURRENT_LIST_DIR}/scripts/doctest.cmake)
include_directories("${PROJECT_SOURCE_DIR}/../Engine")

set(DOCTEST_INCLUDE_DIR Includes/doctest CACHE INTERNAL "Path to include folder for doctest")

set(
  TestList
  MainTests
  FMODTests
  InputTests
  TextRenderTests
)

foreach(name IN LISTS TestList)
  add_executable("TEST_${name}" "${name}.cpp")
  target_link_libraries("TEST_${name}" PUBLIC compiler_flags Engine)
  doctest_discover_tests("TEST_${name}")
endforeach()
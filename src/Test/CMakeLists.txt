project(Tests)
enable_testing()

include(${CMAKE_CURRENT_LIST_DIR}/scripts/doctest.cmake)
include_directories("${PROJECT_SOURCE_DIR}/../Core")

set(DOCTEST_INCLUDE_DIR Includes/doctest CACHE INTERNAL "Path to include folder for doctest")

set(
  TestList
  #MainTests
  #FMODTests
  #InputTests
  #TextRenderTests
  LuaTest
)


add_test(NAME copy_test_data
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/TestData"
    "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/TestData"
)
set_tests_properties(copy_test_data PROPERTIES FIXTURES_SETUP TestDataFixture)

foreach(name IN LISTS TestList)
  add_executable("TEST_${name}" "${name}.cpp")
  
  set_target_properties("TEST_${name}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/$<CONFIG>")
  target_link_libraries("TEST_${name}" PUBLIC Core)
  target_compile_definitions("TEST_${name}" PUBLIC "CTEST")
  doctest_discover_tests("TEST_${name}"
                        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
                        PROPERTIES FIXTURES_REQUIRED TestDataFixture
  )
endforeach()

add_test(NAME remove_test_data
    COMMAND ${CMAKE_COMMAND} -E rm -rf
    "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/TestData"
)
set_tests_properties(remove_test_data PROPERTIES FIXTURES_CLEANUP TestDataFixture)
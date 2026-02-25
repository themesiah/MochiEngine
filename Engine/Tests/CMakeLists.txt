enable_testing()

project (MochiEngineTesting)

include(${CMAKE_CURRENT_LIST_DIR}/scripts/doctest.cmake)

set(DOCTEST_INCLUDE_DIR Includes/doctest CACHE INTERNAL "Path to include folder for doctest")

set(
  TestList
  FMODTests
  InputTests
  TextRenderTests
  LuaTest
  PackerTests
  CameraTests
  TimeTests
  EventBusTests
  TypesTests
  PhysicsTests
  FullEngineTests
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test/$<CONFIG>)

add_test(NAME copy_test_data
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/../TestData"
        "${CMAKE_BINARY_DIR}/test/$<CONFIG>/TestData"
)
set_tests_properties(copy_test_data PROPERTIES FIXTURES_SETUP TestDataFixture)
add_test(NAME copy_test_data2
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/../CoreData"
        "${CMAKE_BINARY_DIR}/test/$<CONFIG>/Data/Core"
)
set_tests_properties(copy_test_data2 PROPERTIES FIXTURES_SETUP TestDataFixture)


foreach(name IN LISTS TestList)
  add_executable("TEST_${name}" "${name}.cpp")
  
  target_link_libraries("TEST_${name}" PUBLIC Engine)
  target_compile_definitions("TEST_${name}" PUBLIC "CTEST")

    if ("${name}" STREQUAL FMODTests)
      add_custom_command(TARGET "TEST_${name}" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
          "${PROJECT_SOURCE_DIR}/../Includes/FMOD/win/studio/lib/x64/fmodstudio.dll"
          "$<TARGET_FILE_DIR:TEST_${name}>/fmodstudio.dll"
        COMMAND ${CMAKE_COMMAND} -E copy
          "${PROJECT_SOURCE_DIR}/../Includes/FMOD/win/core/lib/x64/fmod.dll"
          "$<TARGET_FILE_DIR:TEST_${name}>/fmod.dll")
    endif()
    
  doctest_discover_tests("TEST_${name}"
    PROPERTIES
      FIXTURES_REQUIRED TestDataFixture)
endforeach()
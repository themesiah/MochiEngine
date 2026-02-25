enable_testing()

project(SpaceShooterTesting)

include(${CMAKE_CURRENT_LIST_DIR}/scripts/doctest.cmake)

set(DOCTEST_INCLUDE_DIR Includes/doctest CACHE INTERNAL "Path to include folder for doctest")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SpaceShooter/test/$<CONFIG>)

set(
  SpaceShooterTestList
  SpaceShooterTests
)

add_test(NAME copy_spaceshooter_test_data
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/../TestData"
        "${CMAKE_BINARY_DIR}/SpaceShooter/test/$<CONFIG>/TestData"
)
set_tests_properties(copy_spaceshooter_test_data PROPERTIES FIXTURES_SETUP SpaceshooterTestDataFixture)
add_test(NAME copy_spaceshooter_test_data2
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/../GameData"
        "${CMAKE_BINARY_DIR}/SpaceShooter/test/$<CONFIG>/Data/Game"
)
set_tests_properties(copy_spaceshooter_test_data2 PROPERTIES FIXTURES_SETUP SpaceshooterTestDataFixture)
add_test(NAME copy_spaceshooter_test_data3
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/../../../Engine/CoreData"
        "${CMAKE_BINARY_DIR}/SpaceShooter/test/$<CONFIG>/Data/Core"
)
set_tests_properties(copy_spaceshooter_test_data3 PROPERTIES FIXTURES_SETUP SpaceshooterTestDataFixture)

foreach(name IN LISTS SpaceShooterTestList)
  add_executable("TEST_${name}" "${name}.cpp")
  
  target_link_libraries("TEST_${name}" PUBLIC SpaceShooterLib)
  target_compile_definitions("TEST_${name}" PUBLIC "CTEST")

    add_custom_command(TARGET "TEST_${name}" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
          "${PROJECT_SOURCE_DIR}/../../../Engine/Includes/FMOD/win/studio/lib/x64/fmodstudio.dll"
          "$<TARGET_FILE_DIR:TEST_${name}>/fmodstudio.dll"
        COMMAND ${CMAKE_COMMAND} -E copy
          "${PROJECT_SOURCE_DIR}/../../../Engine/Includes/FMOD/win/core/lib/x64/fmod.dll"
          "$<TARGET_FILE_DIR:TEST_${name}>/fmod.dll"
    )

  doctest_discover_tests("TEST_${name}"
    PROPERTIES
      FIXTURES_REQUIRED SpaceshooterTestDataFixture)
endforeach()
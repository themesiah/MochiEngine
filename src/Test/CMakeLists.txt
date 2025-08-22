# Get all .cpp files
file(GLOB Test_src CONFIGURE_DEPENDS "*.cpp")

enable_testing()

# Create the library
add_executable(Testing ${Test_src})

target_link_libraries(Testing PUBLIC
  compiler_flags
)

set(
    tests
    FirstTest
)

#foreach (name IN LISTS tests)
#    add_test(NAME name COMMAND Test)
#endforeach ()

add_test(
    NAME FirstTest
    COMMAND $<TARGET_FILE:Testing>
    # WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
# Get all .cpp files
file(GLOB Packer_src CONFIGURE_DEPENDS "PackFile.cpp" "SystemFileLoader.cpp")

# Create the library
add_library(PackerStatic ${Packer_src})
target_include_directories(PackerStatic INTERFACE "")
target_compile_features(PackerStatic PRIVATE cxx_std_23)

# Create the handy executable to pack folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/packer/$<CONFIG>)
add_executable(Packer "Packer.cpp" ${Packer_src})
target_compile_features(Packer PRIVATE cxx_std_23)
cmake_minimum_required(VERSION 4.0)


# Set the project name and version
project(MochiEngineSamples VERSION 0.1)


set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static-libgcc -static-libstdc++") # <-- Only for release!

enable_testing()


# Add all subdirectories
add_subdirectory("Samples/SpaceShooter")
#add_subdirectory("Engine")
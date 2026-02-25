project(MochiEngine)
# Get all .cpp files
file(GLOB Engine_src CONFIGURE_DEPENDS
        "src/Graphics/*.cpp"
        "src/Graphics/SDL/*.cpp"
        "src/Audio/*.cpp"
        "src/Input/*.cpp"
        "src/Utils/*.cpp"
        "src/Time/*.cpp"
        "src/Event/EventBus.h"
        "src/Event/*.cpp"
        "src/GUI/*.cpp"
        "src/Types/Types.hpp"
        "src/Physics/*.cpp"
        "src/Physics/*.hpp"
        "src/Scripting/*.cpp"
        "src/*.hpp"
        "src/*.cpp")

file(GLOB Debug_src CONFIGURE_DEPENDS "src/Debug/*.cpp")

set(LUA_VERSION "lua-5.4.7")
add_subdirectory(Includes)

add_subdirectory(Includes/FMOD EXCLUDE_FROM_ALL)
add_subdirectory(src/Packer)

add_library(Engine ${Engine_src} $<$<CONFIG:Debug>:${Debug_src}>)
target_compile_features(Engine PUBLIC cxx_std_23)
target_compile_definitions(Engine PUBLIC $<$<CONFIG:Debug>:DEBUG>)

target_include_directories(Engine PUBLIC "Includes/FMOD")
target_include_directories(Engine INTERFACE "src")

target_link_libraries(Engine
                      PUBLIC
                      EngineDependencies
                      fmod
                      fmodstudio
                      PackerStatic
)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(Engine PUBLIC
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-Wno-unused-variable;-Wno-unused-parameter>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

add_custom_command(
  TARGET Engine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  "${PROJECT_SOURCE_DIR}/Includes/FMOD/win/studio/lib/x64/fmodstudio.dll"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fmodstudio.dll"
  COMMAND ${CMAKE_COMMAND} -E copy
  "${PROJECT_SOURCE_DIR}/Includes/FMOD/win/core/lib/x64/fmod.dll"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fmod.dll"
  COMMENT "Copying fmodstudio.dll and fmod.dll"
)

add_custom_command(
  TARGET Engine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${PROJECT_SOURCE_DIR}/CoreData"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data/Core"
  COMMENT "Copying data"
)

add_subdirectory(Tests)
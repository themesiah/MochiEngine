project(MochiEngine)
# Get all .cpp files
file(GLOB Engine_src CONFIGURE_DEPENDS
        "Graphics/*.cpp"
        "Audio/*.cpp"
        "Input/*.cpp"
        "Utils/*.cpp"
        "Time/*.cpp"
        "Event/EventBus.h"
        "GUI/*.cpp"
        "Types/Types.hpp"
        "*.cpp")


set(SDLTTF_VENDORED ON)
set(LUA_VERSION "lua-5.4.8")
add_subdirectory(Includes/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(Includes/SDL3_image EXCLUDE_FROM_ALL)
add_subdirectory(Includes/SDL3_ttf EXCLUDE_FROM_ALL)
add_subdirectory(Includes/FMOD EXCLUDE_FROM_ALL)
add_subdirectory(Includes/lua EXCLUDE_FROM_ALL)
add_subdirectory(Includes/sol2 EXCLUDE_FROM_ALL)
add_subdirectory(Packer)

add_subdirectory(Tests)

add_library(Engine ${Engine_src})
target_compile_features(Engine PUBLIC cxx_std_23)
target_compile_definitions(Engine PUBLIC $<$<CONFIG:Debug>:DEBUG>)

target_include_directories(Engine PUBLIC "Includes/FMOD")
target_include_directories(Engine PUBLIC "Includes/Json")
target_include_directories(Engine INTERFACE "")

target_link_libraries(Engine
                      PUBLIC
                      SDL3_image::SDL3_image
                      SDL3::SDL3
                      SDL3_ttf::SDL3_ttf
                      fmod
                      fmodstudio
                      sol2
                      lua_static
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
  COMMENT "Copying fmodstudio.dll"
)

add_custom_command(
  TARGET Engine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  "${PROJECT_SOURCE_DIR}/Includes/FMOD/win/core/lib/x64/fmod.dll"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fmod.dll"
  COMMENT "Copying fmod.dll"
)
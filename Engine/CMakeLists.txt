project(MochiEngine)

# Get all .cpp files in the engine
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
        "src/ECS/*.cpp"
        "src/ECS/Systems/*.cpp"
        "src/*.hpp"
        "src/*.cpp")

# Get debug .cpp files to use only on debug builds
file(GLOB Debug_src CONFIGURE_DEPENDS "src/Debug/*.cpp")

# Lua version needed to automatically download lua and use it with sol2
set(LUA_VERSION "lua-5.4.7")
add_subdirectory(Includes)

add_subdirectory(src/Packer)

# Adds source files and debug source files only if in debug
add_library(Engine ${Engine_src} $<$<CONFIG:Debug>:${Debug_src}>)
target_compile_features(Engine PUBLIC cxx_std_23)
# A "DEBUG" definition is created and available in code if the game is in debug
target_compile_definitions(Engine PUBLIC $<$<CONFIG:Debug>:DEBUG>)

target_include_directories(Engine INTERFACE "src")

target_link_libraries(Engine
                      PUBLIC
                      EngineDependencies
                      PackerStatic
)

if (USE_FMOD)
  add_subdirectory(Includes/FMOD EXCLUDE_FROM_ALL)
  target_include_directories(Engine PUBLIC "Includes/FMOD")
  target_compile_definitions(Engine PUBLIC "USE_FMOD")
  target_link_libraries(Engine
                        PUBLIC
                        fmod
                        fmodstudio)
endif()

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(Engine PUBLIC
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-Wno-unused-variable;-Wno-unused-parameter>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

if(ENABLE_TESTS)
add_subdirectory(Tests)
endif()
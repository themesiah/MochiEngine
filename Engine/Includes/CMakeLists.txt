project(EngineDependencies)

include (FetchContent)

add_library(${PROJECT_NAME} INTERFACE)


set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(SDLIMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_TIF_SHARED OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_WEBP_SHARED OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_WEBP OFF CACHE BOOL "" FORCE)

set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
set(SDLTTF_HARFBUZZ OFF CACHE BOOL "" FORCE)

set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_TESTS OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_TESTS_INSTALL OFF CACHE BOOL "" FORCE)
set(SDLTTF_TEST OFF CACHE BOOL "" FORCE)
set(SDL_CPU_FEATURE_DETECTION OFF CACHE BOOL "" FORCE)
set(SDL_ASM_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.2
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-3.4.0
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3_image)

FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-3.2.2
)
FetchContent_MakeAvailable(SDL3_ttf)

add_subdirectory(Lua)

FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(sol2)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.16.0
)
FetchContent_MakeAvailable(EnTT)


target_link_libraries(${PROJECT_NAME}
                    INTERFACE
                    SDL3::SDL3-static
                    SDL3_image::SDL3_image-static
                    SDL3_ttf::SDL3_ttf-static
                    lua
                    sol2
                    nlohmann_json
                    EnTT::EnTT
)

if (NOT USE_FMOD)
    set(SDLMIXER_MIDI_NATIVE OFF)     # disable formats we don't use to make the build faster and smaller. Also some of these don't work on all platforms so you'll need to do some experimentation.
    set(SDLMIXER_GME OFF)
    set(SDLMIXER_WAVPACK OFF)     
    set(SDLMIXER_MOD OFF)
    set(SDLMIXER_MOD_XMP OFF)
    set(SDLMIXER_OPUS OFF)
    # set(SDLMIXER_MP3_MPG123 OFF)
    # set(SDLMIXER_VORBIS_VORBISFILE OFF)
    # set(SDLMIXER_FLAC_LIBFLAC OFF)

    set(SDLMIXER_VENDORED ON CACHE BOOL "" FORCE)
    set(SDLMIXER_TESTS OFF CACHE BOOL "" FORCE)
    set(SDLMIXER_TESTS_INSTALL OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        SDL3_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG prerelease-3.1.2
        OVERRIDE_FIND_PACKAGE
    )
    FetchContent_MakeAvailable(SDL3_mixer)

    target_link_libraries(${PROJECT_NAME}
                        INTERFACE
                        SDL3_mixer::SDL3_mixer-static)
endif()
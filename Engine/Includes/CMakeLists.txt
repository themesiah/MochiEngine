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

set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
set(SDLTTF_HARFBUZZ OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.22
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-3.2.4
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3_image)

FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-3.2.2
)
FetchContent_MakeAvailable(SDL3_ttf)


target_link_libraries(${PROJECT_NAME}
                    INTERFACE
                    SDL3::SDL3-static
                    SDL3_image::SDL3_image-static
                    SDL3_ttf::SDL3_ttf-static
)
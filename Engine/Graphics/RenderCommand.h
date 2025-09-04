#ifndef HDEF_RENDERCOMMAND
#define HDEF_RENDERCOMMAND

#include <SDL3/SDL.h>
#include <memory>

struct RenderCommand
{
    std::shared_ptr<SDL_Texture> texture;
    SDL_FRect sourceRect;
    SDL_FRect destRect;
    unsigned int zindex;
};

#endif
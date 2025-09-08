#ifndef HDEF_RENDERCOMMAND
#define HDEF_RENDERCOMMAND

#include <memory>
#include "../Types/Types.hpp"

namespace Mochi::Graphics
{
    struct RenderCommand
    {
        std::shared_ptr<SDL_Texture> texture;
        Rectf sourceRect;
        Rectf destRect;
        unsigned int zindex;
    };
}

#endif
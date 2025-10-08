#ifndef HDEF_RENDERCOMMAND
#define HDEF_RENDERCOMMAND

#include <memory>

#include "../Types/Types.hpp"
#include "ITexture.h"

namespace Mochi::Graphics
{
    struct RenderCommand
    {
        ITexture *texture;
        Rectf sourceRect;
        Rectf destRect;
        unsigned int zindex;
    };
}

#endif
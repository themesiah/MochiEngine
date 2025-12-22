#ifndef HDEF_RENDERCOMMAND
#define HDEF_RENDERCOMMAND

#include <memory>
#include <optional>

#include "../Types/Types.hpp"
#include "ITexture.h"

namespace Mochi::Graphics
{
    enum RenderCommandPositionType
    {
        RenderCommandPositionWorld,
        RenderCommandPositionScreen
    };

    struct TiledRenderOptions
    {
        float Scale;
    };

    struct RenderCommand
    {
        ITexture *texture;
        Rectf sourceRect;
        Rectf destRect;
        unsigned int zindex;
        RenderCommandPositionType PositionType = RenderCommandPositionWorld;
        std::optional<TiledRenderOptions> TiledRenderOptions;
    };
}

#endif
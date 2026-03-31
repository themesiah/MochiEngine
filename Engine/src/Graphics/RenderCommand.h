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

    enum RenderCommandFlipMode
    {
        None,
        Vertical,
        Horizontal,
        VerticalAndHorizontal
    };

    struct TiledRenderOptions
    {
        float Scale;
    };

    /// @brief Defines a full rendering unit, which for the purpose of the engine is a texture (or part of it) on a specific point in space.
    /// Additionaly, it allows to present the texture based on the world or the screen position and in a tiled format, if needed.
    /// If TiledRenderOptions is not set, the texture will not be tiled.
    struct RenderCommand
    {
        ITexture *texture;
        Rectf sourceRect;
        Rectf destRect;
        unsigned int zindex;
        double angle = 0.0;
        RenderCommandFlipMode flip = RenderCommandFlipMode::None;
        RenderCommandPositionType PositionType = RenderCommandPositionWorld;
        std::optional<TiledRenderOptions> TiledRenderOptions;
    };
}

#endif
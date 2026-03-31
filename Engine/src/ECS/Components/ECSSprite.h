#ifndef HDEF_ECSSPRITE
#define HDEF_ECSSPRITE

#include "../../Types/Types.hpp"
#include "../../Graphics/ITexture.h"
#include "../../Graphics/RenderCommand.h"

namespace Mochi::ECS
{
    struct SpriteComponent
    {
        Graphics::ITexture *Texture;
        int ZIndex;
        Rectf SrcRect{{0.0f, 0.0f}, Texture->GetSize()};
        double Angle = 0.0;
        Graphics::RenderCommandFlipMode Flip = Graphics::RenderCommandFlipMode::None;
    };
}

#endif
#ifndef HDEF_BACKGROUND
#define HDEF_BACKGROUND

#include <string>

#include "Graphics/SpriteBase.h"
#include "Graphics/AbstractTextureFactory.h"
#include "Graphics/RenderCommand.h"

namespace Mochi::Shooter
{
    class Background : Graphics::SpriteBase
    {
    public:
        Background(Graphics::AbstractTextureFactory *);
        virtual std::vector<Graphics::RenderCommand> GetRenderData() const override;
    };
}

#endif
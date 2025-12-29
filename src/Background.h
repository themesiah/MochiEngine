#ifndef HDEF_BACKGROUND
#define HDEF_BACKGROUND

#include <string>

#include "Graphics/SpriteBase.h"
#include "Graphics/AbstractTextureFactory.h"
#include "Graphics/RenderCommand.h"

namespace Mochi::Shooter
{
    class Background : public Graphics::SpriteBase
    {
    private:
        float mXPosition;

    public:
        Background(Graphics::AbstractTextureFactory *);
        virtual std::vector<Graphics::RenderCommand> GetRenderData() const override;
        float GetXPosition() const;
        void SetXPosition(const float &xPosition);
    };
}

#endif
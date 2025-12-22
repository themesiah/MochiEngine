#include "Background.h"

#include <string>

#include "Graphics/SpriteBase.h"
#include "Graphics/AbstractTextureFactory.h"
#include "Graphics/RenderCommand.h"
#include "Constants.h"

#include "ZIndexEnum.h"

namespace Mochi::Shooter
{
    inline const std::string BACKGROUND_PATH = "Background.png";

    Background::Background(Graphics::AbstractTextureFactory *textureFactory) : Graphics::SpriteBase(textureFactory, BACKGROUND_PATH)
    {
        SetZIndex(ZINDEX_BACKGROUND);
        mTransform->Scale = 2.0f;
    }

    std::vector<Graphics::RenderCommand> Background::GetRenderData() const
    {
        if (!mTexture)
            throw EngineError("A texture was not initialized");
        if (!mVisible)
            return {};

        auto height = mTexture->GetSize().y;
        Graphics::RenderCommand bottom;
        bottom.texture = mTexture.get();
        bottom.zindex = mZindex;
        bottom.PositionType = Graphics::RenderCommandPositionType::RenderCommandPositionScreen;
        bottom.TiledRenderOptions = Graphics::TiledRenderOptions{mTransform->Scale};
        Graphics::RenderCommand center = bottom;
        Graphics::RenderCommand top = bottom;

        float limitHeightSrc = 6.0f;
        float limitWidthSrc = 8.0f;
        float limitHeightDst = limitHeightSrc * mTransform->Scale;
        bottom.sourceRect = Rectf(0.0f, height - limitHeightSrc, limitWidthSrc, limitHeightSrc);
        bottom.destRect = Rectf(0.0f, CONST_RENDER_LOGICAL_Y - limitHeightDst, CONST_RENDER_LOGICAL_X, limitHeightDst);
        top.sourceRect = Rectf(0.0f, 0.0f, limitWidthSrc, limitHeightSrc);
        top.destRect = Rectf(0.0f, 0.0f, CONST_RENDER_LOGICAL_X, limitHeightDst);
        center.sourceRect = Rectf(0.0f, limitHeightSrc, limitWidthSrc, height - limitHeightSrc * 2.0f);
        center.destRect = Rectf(0.0f, limitHeightDst, CONST_RENDER_LOGICAL_X, CONST_RENDER_LOGICAL_Y - limitHeightDst * 2.0f);
        return {bottom, center, top};
    }
}
#include "Background.h"

#include <string>

#include "Graphics/SpriteBase.h"
#include "Graphics/AbstractTextureFactory.h"
#include "Graphics/RenderCommand.h"
#include "Constants.h"
#include "Utils/MathUtils.h"
#include "Utils/Logger.h"

#include "ZIndexEnum.h"

namespace Mochi::Shooter
{
    inline const std::string BACKGROUND_PATH = "Background.png";

    Background::Background(Graphics::AbstractTextureFactory *textureFactory) : Graphics::SpriteBase(textureFactory, BACKGROUND_PATH), mXPosition(0.0f)
    {
        SetZIndex(ZINDEX_BACKGROUND);
        mTransform->Scale = 2.0f;
    }

    float Background::GetXPosition() const
    {
        return mXPosition;
    }

    void Background::SetXPosition(const float &xPosition)
    {
        mXPosition = xPosition;
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
        center.TiledRenderOptions = Graphics::TiledRenderOptions(1.0f);
        Graphics::RenderCommand top = bottom;

        float limitHeightSrc = 6.0f;
        float limitWidthSrc = 8.0f;

        auto xpos = Math::Lerp(0.0f, mTexture->GetSize().x - limitWidthSrc, mXPosition);

        float limitHeightDst = limitHeightSrc * mTransform->Scale;
        bottom.sourceRect = Rectf(xpos, height - limitHeightSrc, limitWidthSrc, limitHeightSrc);
        bottom.destRect = Rectf(0.0f, CONST_RENDER_LOGICAL_Y - limitHeightDst, CONST_RENDER_LOGICAL_X, limitHeightDst);
        top.sourceRect = Rectf(xpos, 0.0f, limitWidthSrc, limitHeightSrc);
        top.destRect = Rectf(0.0f, 0.0f, CONST_RENDER_LOGICAL_X, limitHeightDst);
        center.sourceRect = Rectf(xpos, limitHeightSrc, limitWidthSrc, height - limitHeightSrc * 2.0f);
        center.destRect = Rectf(0.0f, limitHeightDst, CONST_RENDER_LOGICAL_X, CONST_RENDER_LOGICAL_Y - limitHeightDst * 2.0f);

        return {bottom, center, top};
    }
}
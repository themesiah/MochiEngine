#include "SpriteBase.h"

#include <memory>
#include <string>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <format>

#include "../Utils/Logger.h"
#include "../Packer/PackCatalog.h"
#include "AbstractTextureFactory.h"
#include "../Exception.hpp"
#include "../Utils/Assert.h"

namespace Mochi::Graphics
{
    SpriteBase::SpriteBase(AbstractTextureFactory *textureFactory, const std::string &filename)
        : mZindex(0), mVisible(true), mAlpha(255), mTransform(std::make_shared<Transform>())
    {
        LoadTexture(textureFactory, filename);
    }

    SpriteBase::SpriteBase() : mZindex(0), mVisible(true), mAlpha(255), mTransform(std::make_shared<Transform>())
    {
        mSrcRect.SetPosition({0.0f, 0.0f});
    }

    void SpriteBase::LoadTexture(AbstractTextureFactory *textureFactory, const std::string &filename)
    {
        mTexture = textureFactory->GetTexture(filename);

        ASSERT("Texture data was not loaded", mTexture != nullptr);

        mSize = mTexture->GetSize();

        mSrcRect.w = mSize.x;
        mSrcRect.h = mSize.y;
    }

    std::vector<RenderCommand> SpriteBase::GetRenderData() const
    {
        if (!mTexture)
            throw EngineError("A texture was not initialized");
        if (!mVisible)
            return {};
        RenderCommand rc;
        rc.texture = mTexture.get();
        rc.sourceRect = mSrcRect;
        rc.destRect.SetPosition(mTransform->GetPosition());
        rc.destRect.w = mSize.x * mTransform->GetScale();
        rc.destRect.h = mSize.y * mTransform->GetScale();
        rc.zindex = mZindex;
        return {rc};
    }

    void SpriteBase::Update(const float &dt) {}

    void SpriteBase::SetSrcRect(const Rectf &srcRect)
    {
        mSrcRect = srcRect;
    }

    SpriteBase::~SpriteBase()
    {
    }

    void SpriteBase::SetZIndex(const uint16_t &zIndex)
    {
        mZindex = zIndex;
    }

    uint16_t SpriteBase::GetZIndex() const
    {
        return mZindex;
    }

    bool SpriteBase::IsVisible() const
    {
        return mVisible;
    }

    void SpriteBase::SetVisible(const bool &visible)
    {
        mVisible = visible;
    }

    uint8_t SpriteBase::GetAlpha() const
    {
        return mAlpha;
    }

    void SpriteBase::SetAlpha(const uint8_t &alpha)
    {
        mAlpha = alpha;
        mTexture->SetAlpha(alpha);
    }

    std::shared_ptr<Transform> SpriteBase::GetTransform() const
    {
        return mTransform;
    }

    void SpriteBase::SetTransform(std::shared_ptr<Transform> tr)
    {
        mTransform = tr;
    }
}

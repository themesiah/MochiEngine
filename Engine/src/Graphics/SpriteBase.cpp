#include "SpriteBase.h"

#include <memory>
#include <string>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <format>

#include "../Utils/Logger.h"
#include "../Packer/PackCatalog.h"
#include "TextureFactory.h"
#include "../Exception.hpp"
#include "../Utils/Assert.h"

namespace Mochi::Graphics
{
    SpriteBase::SpriteBase(std::shared_ptr<TextureFactory> textureFactory, const std::string &filename) : mScale(1.0f), mPosition(0.0f, 0.0f), mZindex(0)
    {
        LoadTexture(textureFactory, filename);
    }

    SpriteBase::SpriteBase() : mScale(1.0f), mZindex(0)
    {
        mSrcRect.SetPosition({0.0f, 0.0f});
    }

    void SpriteBase::LoadTexture(std::shared_ptr<TextureFactory> textureFactory, const std::string &filename)
    {
        mTexture = textureFactory->GetTexture(filename);

        ASSERT("Texture data was not loaded", mTexture != nullptr);

        float w, h;
        SDL_GetTextureSize(mTexture.get(), &w, &h);
        mSize = Vector2f(w, h);

        mSrcRect.w = w;
        mSrcRect.h = h;
    }

    RenderCommand SpriteBase::GetRenderData() const
    {
        if (!mTexture)
            throw EngineError("A texture was not initialized");
        RenderCommand rc;
        rc.texture = mTexture;
        rc.sourceRect = mSrcRect;
        rc.destRect.SetPosition(mPosition);
        rc.destRect.w = mSize.x * mScale;
        rc.destRect.h = mSize.y * mScale;
        rc.zindex = mZindex;
        return rc;
    }

    void SpriteBase::SetSrcRect(const Rectf &srcRect)
    {
        mSrcRect = srcRect;
    }

    SpriteBase::~SpriteBase()
    {
    }

    void SpriteBase::SetScale(const float &scale)
    {
        mScale = scale;
    }

    float SpriteBase::GetScale() const
    {
        return mScale;
    }

    void SpriteBase::SetPosition(const Vector2f &position)
    {
        mPosition = position;
    }

    Vector2f SpriteBase::GetPosition() const
    {
        return mPosition;
    }

    void SpriteBase::SetZIndex(const uint16_t &zIndex)
    {
        mZindex = zIndex;
    }

    uint16_t SpriteBase::GetZIndex() const
    {
        return mZindex;
    }
}

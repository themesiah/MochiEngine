#include "Sprite.h"

#include <memory>
#include <string>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <format>

#include "../Utils/Logger.h"
#include "../Packer/PackCatalog.h"
#include "TextureFactory.h"

namespace Mochi::Graphics
{
    Sprite::Sprite(std::shared_ptr<TextureFactory> textureFactory, const std::string &filename)
    {
        mTexture = textureFactory->GetTexture(filename);

        mScale = 1;
        float w, h;
        SDL_GetTextureSize(mTexture.get(), &w, &h);

        mSrcRect.x = 0;
        mSrcRect.y = 0;
        mSrcRect.w = w;
        mSrcRect.h = h;

        mDestRect.w = w;
        mDestRect.h = h;
        mDestRect.x = 0;
        mDestRect.y = 0;
    }

    std::vector<RenderCommand> Sprite::GetRenderData() const
    {
        RenderCommand rc;
        rc.texture = mTexture;
        rc.sourceRect = mSrcRect;
        rc.destRect = mDestRect;
        rc.destRect.w *= mScale;
        rc.destRect.h *= mScale;
        rc.zindex = 1; // TEMP
        return {rc};
    }

    Sprite::~Sprite()
    {
    }

    void Sprite::SetScale(const float &scale)
    {
        mScale = scale;
    }

    float Sprite::GetScale() const
    {
        return mScale;
    }
}

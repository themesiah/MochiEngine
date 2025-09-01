#include "Sprite.h"

#include <memory>
#include <string>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <format>

#include "../Logger.h"
#include "../Packer/PackCatalog.h"
#include "TextureFactory.h"

Sprite::Sprite(std::shared_ptr<TextureFactory> textureFactory, const std::string &filename)
{
    mTexture = textureFactory->GetTexture(filename);

    mScale = 2;
    float w, h;
    SDL_GetTextureSize(mTexture.get(), &w, &h);

    mSrcRect.x = 0;
    mSrcRect.y = 0;
    mSrcRect.w = w;
    mSrcRect.h = h;

    mDestRect.w = w * mScale;
    mDestRect.h = h * mScale;
    mDestRect.x = 0;
    mDestRect.y = 0;
}

RenderCommand Sprite::GetRenderData() const
{
    RenderCommand rc;
    rc.texture = mTexture;
    rc.sourceRect = mSrcRect;
    rc.destRect = mDestRect;
    rc.zindex = 1; // TEMP
    return rc;
}

Sprite::~Sprite()
{
}
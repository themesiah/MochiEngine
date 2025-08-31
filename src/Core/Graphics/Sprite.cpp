#include "Sprite.h"

#include <memory>
#include <string>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <format>

#include "../Logger.h"
#include "../Packer/PackCatalog.h"
#include "TextureFactory.h"

Sprite::Sprite(std::shared_ptr<TextureFactory> textureFactory, SDL_Renderer *renderer, const std::string &filename) : mRenderer(renderer)
{
    mTexture = textureFactory->GetTexture(filename);

    mScale = 0.5f;

    int w = 0;
    int h = 0;

    SDL_GetRenderOutputSize(renderer, &w, &h);
    SDL_GetTextureSize(mTexture.get(), &mDestRect.w, &mDestRect.h);
    mDestRect.x = ((w / mScale) - mDestRect.w) / 2;
    mDestRect.y = ((h / mScale) - mDestRect.h) / 2;

    mSrcRect.x = 0;
    mSrcRect.y = 0;
    mSrcRect.w = mDestRect.w;
    mSrcRect.h = mDestRect.h;
}

void Sprite::Render(SDL_Renderer *renderer) const
{
    SDL_SetRenderScale(renderer, mScale, mScale);
    SDL_RenderTexture(renderer, mTexture.get(), &mSrcRect, &mDestRect);
}

Sprite::~Sprite()
{
}
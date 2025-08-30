#include "Sprite.h"

#include <iostream>
#include <SDL3_image/SDL_image.h>

Sprite::Sprite(SDL_Renderer *renderer, std::vector<char> buffer)
{
    mTexture = std::shared_ptr<SDL_Texture>(IMG_LoadTexture_IO(renderer, SDL_IOFromConstMem(buffer.data(), buffer.size()), true), SDL_DestroyTexture);
    if (!mTexture)
    {
        std::cout << "Image not found!" << std::endl;
        std::cout << "Error is: " << SDL_GetError() << std::endl;
    }

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
    SDL_DestroyTexture(mTexture.get());
}
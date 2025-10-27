#include "SDLTexture.h"

#include "SDL3/SDL.h"

namespace Mochi::Graphics
{
    SDLTexture::SDLTexture(SDL_Texture *texture) : mTexture(texture)
    {
    }

    SDLTexture::~SDLTexture()
    {
        SDL_DestroyTexture(mTexture);
    }

    SDL_Texture *SDLTexture::GetTexture() const
    {
        return mTexture;
    }

    Vector2f SDLTexture::GetSize() const
    {
        Vector2f result;
        SDL_GetTextureSize(mTexture, &result.x, &result.y);
        return result;
    }

    void SDLTexture::SetAlpha(const uint8_t &alpha)
    {
        SDL_SetTextureAlphaMod(mTexture, alpha);
    }
}
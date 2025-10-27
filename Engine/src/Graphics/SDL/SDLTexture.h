#ifndef HDEF_SDLTEXTURE
#define HDEF_SDLTEXTURE

#include "../ITexture.h"

#include "SDL3/SDL.h"

#include "../../Types/Types.hpp"

namespace Mochi::Graphics
{
    class SDLTexture : public ITexture
    {
    public:
        SDLTexture(SDL_Texture *texture);
        virtual ~SDLTexture();
        virtual Vector2f GetSize() const override;
        SDL_Texture *GetTexture() const;
        virtual void SetAlpha(const uint8_t &alpha) override;

    private:
        SDL_Texture *mTexture;
    };
}

#endif
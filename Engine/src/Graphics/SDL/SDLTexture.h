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
        virtual Vector2f GetSize() const;
        SDL_Texture *GetTexture() const;

    private:
        SDL_Texture *mTexture;
    };
}

#endif
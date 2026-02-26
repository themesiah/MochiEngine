#ifndef HDEF_SDLTEXTURE
#define HDEF_SDLTEXTURE

#include "../ITexture.h"

#include "SDL3/SDL.h"

#include "../../Types/Types.hpp"

namespace Mochi::Graphics
{
    /**
     * @brief An implementation of ITexture used by SDLRenderer.
     *
     * NOTE: Do not mistake SDLTexture (this) with SDL_Texture (the struct used internally by SDL)
     *
     * Ownership:
     *
     * - SDLTexture has complete ownership of SDL_Texture. It is SDLTexture which is shared between factories and sprites.
     *
     * Guarantees:
     *
     * - The SDL_Texture will be destroyed at the end of SDLTexture lifetime.
     */
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
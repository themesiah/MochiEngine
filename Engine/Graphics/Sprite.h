#ifndef HDEF_SPRITE
#define HDEF_SPRITE

#include "../Entity/IRenderable.h"

#include <memory>
#include <vector>
#include <string>
#include <SDL3/SDL.h>

#include "../Entity/IEntity.h"

namespace Mochi
{
    namespace Graphics
    {
        class TextureFactory;
        class Sprite : public IRenderable, public IEntity
        {
        private:
            std::shared_ptr<SDL_Texture> mTexture;
            SDL_FRect mDestRect;
            SDL_FRect mSrcRect;
            float mScale;

        public:
            Sprite(std::shared_ptr<TextureFactory>, const std::string &filename);
            virtual ~Sprite();
            virtual RenderCommand GetRenderData() const;
        };
    }
}

#endif
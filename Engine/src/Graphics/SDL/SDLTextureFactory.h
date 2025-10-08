#ifndef HDEF_TEXTUREFACTORY
#define HDEF_TEXTUREFACTORY

#include <memory>
#include <unordered_map>
#include <string>

#include "../AbstractTextureFactory.h"

#include "SDL3/SDL.h"

// struct SDL_Renderer;
namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Graphics
{
    class SDLRenderer;
    class SDLTextureFactory : public AbstractTextureFactory
    {
    private:
        friend class SDLRenderer;
        SDLTextureFactory(FS::PackCatalog *catalog, SDL_Renderer *renderer);
        FS::PackCatalog *mCatalog;
        SDL_Renderer *mRenderer;

    protected:
        virtual std::shared_ptr<ITexture> LoadTexture(const std::string &texturePath) override;

    public:
        virtual ~SDLTextureFactory();
    };
}

#endif
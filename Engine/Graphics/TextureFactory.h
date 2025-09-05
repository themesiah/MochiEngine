#ifndef HDEF_TEXTUREFACTORY
#define HDEF_TEXTUREFACTORY

#include <memory>
#include <unordered_map>
#include <string>

struct SDL_Texture;
struct SDL_Renderer;
namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Graphics
{
    class TextureFactory
    {
    private:
        std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> mTexturesMap;
        std::shared_ptr<FS::PackCatalog> mCatalog;
        std::shared_ptr<SDL_Renderer> mRenderer;

    public:
        TextureFactory(std::shared_ptr<FS::PackCatalog>, std::shared_ptr<SDL_Renderer>);
        ~TextureFactory();
        std::shared_ptr<SDL_Texture> GetTexture(const std::string &texturePath);
        void DeleteCache();
        void DeleteTexture(const std::string &texturePath);
    };
}

#endif
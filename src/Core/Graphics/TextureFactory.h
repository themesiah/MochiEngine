#ifndef HDEF_TEXTUREFACTORY
#define HDEF_TEXTUREFACTORY

#include <memory>
#include <unordered_map>
#include <string>

struct SDL_Texture;
struct SDL_Renderer;
class PackCatalog;
class TextureFactory
{
private:
    std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> mTexturesMap;
    std::shared_ptr<PackCatalog> mCatalog;
    SDL_Renderer *mRenderer;

public:
    TextureFactory(std::shared_ptr<PackCatalog>, SDL_Renderer *);
    ~TextureFactory();
    std::shared_ptr<SDL_Texture> GetTexture(const std::string &texturePath);
    void DeleteCache();
    void DeleteTexture(const std::string &texturePath);
};

#endif
#include "TextureFactory.h"

#include <SDL3_image/SDL_image.h>
#include <format>

#include "../Packer/PackCatalog.h"
#include "../Assert.h"
#include "../Utils/StringUtils.h"
#include "../Logger.h"

TextureFactory::TextureFactory(std::shared_ptr<PackCatalog> catalog, SDL_Renderer *renderer) : mTexturesMap(), mCatalog(catalog), mRenderer(renderer)
{
}

TextureFactory::~TextureFactory()
{
}

/// @brief Gets a texture from the cache if it exists, or loads it from the catalog if not.
/// @param texturePath The texture path in the catalog
/// @return A pointer to the texture
std::shared_ptr<SDL_Texture> TextureFactory::GetTexture(const std::string &texturePath)
{
    std::string normalizedPath = NormalizePath(texturePath);
    if (mTexturesMap.find(normalizedPath) != mTexturesMap.end())
    {
        return mTexturesMap[normalizedPath];
    }

    auto textureBuffer = mCatalog->GetFile(normalizedPath);
    auto tex = std::shared_ptr<SDL_Texture>(IMG_LoadTexture_IO(mRenderer, SDL_IOFromConstMem(textureBuffer.data(), textureBuffer.size()), true), SDL_DestroyTexture);
    ASSERT("Failed loading texture from the catalog", tex != nullptr);
    SDL_SetTextureScaleMode(tex.get(), SDL_ScaleMode::SDL_SCALEMODE_NEAREST); // SUPER IMPORTANT!
    mTexturesMap[normalizedPath] = tex;
    return tex;
}

/// @brief Clears the textures map, releasing all textures that don't have a reference in any other place. Is safe to call whenever.
void TextureFactory::DeleteCache()
{
    mTexturesMap.clear();
}

/// @brief Removes a texture from the cache, releasing it if it doesn't have any reference in any other place. Is safe to call whenever.
/// @param texturePath Path to the texture inside the catalog.
void TextureFactory::DeleteTexture(const std::string &texturePath)
{
    std::string normalizedPath = NormalizePath(texturePath);
    if (mTexturesMap.find(normalizedPath) != mTexturesMap.end())
    {
        mTexturesMap.erase(normalizedPath);
    }
    else
    {
        LOG_WARNING(std::format("Trying to delete texture with path {} that was not cached.", texturePath));
    }
}

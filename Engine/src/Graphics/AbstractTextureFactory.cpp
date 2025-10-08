#include "AbstractTextureFactory.h"

#include <format>

#include "../Utils/Assert.h"
#include "../Utils/StringUtils.h"
#include "../Utils/Logger.h"
#include "../Exception.hpp"

namespace Mochi::Graphics
{
    AbstractTextureFactory::AbstractTextureFactory() : mTexturesMap()
    {
    }

    AbstractTextureFactory::~AbstractTextureFactory()
    {
    }

    std::shared_ptr<ITexture> AbstractTextureFactory::GetTexture(const std::string &texturePath)
    {
        std::string normalizedPath = Utils::NormalizePath(texturePath);
        if (mTexturesMap.find(normalizedPath) != mTexturesMap.end())
        {
            return mTexturesMap[normalizedPath];
        }
        else
        {
            auto newTexture = LoadTexture(texturePath);
            mTexturesMap[texturePath] = newTexture;
            return newTexture;
        }
    }

    /// @brief Clears the textures map, releasing all textures that don't have a reference in any other place. Is safe to call whenever.
    void AbstractTextureFactory::DeleteCache()
    {
        mTexturesMap.clear();
    }

    /// @brief Removes a texture from the cache, releasing it if it doesn't have any reference in any other place. Is safe to call whenever.
    /// @param texturePath Path to the texture inside the catalog.
    void AbstractTextureFactory::DeleteTexture(const std::string &texturePath)
    {
        std::string normalizedPath = Utils::NormalizePath(texturePath);
        if (mTexturesMap.find(normalizedPath) != mTexturesMap.end())
        {
            mTexturesMap.erase(normalizedPath);
        }
        else
        {
            LOG_WARNING(std::format("Trying to delete texture with path {} that was not cached.", texturePath));
        }
    }
}
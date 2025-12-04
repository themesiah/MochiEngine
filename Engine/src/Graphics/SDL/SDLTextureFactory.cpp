#include "SDLTextureFactory.h"
#include "SDLRenderer.h"

#include "SDL3_image/SDL_image.h"
#include <format>
#include <memory>

#include "SDLTexture.h"
#include "../Packer/PackCatalog.h"
#include "../Utils/Assert.h"
#include "../Utils/StringUtils.h"
#include "../Utils/Logger.h"
#include "../Exception.hpp"

namespace Mochi::Graphics
{
    SDLTextureFactory::SDLTextureFactory(FS::PackCatalog *catalog, SDL_Renderer *renderer) : AbstractTextureFactory(), mCatalog(catalog), mRenderer(renderer)
    {
    }

    SDLTextureFactory::~SDLTextureFactory()
    {
    }

    /// @brief Gets a texture from the cache if it exists, or loads it from the catalog if not.
    /// @param texturePath The texture path in the catalog
    /// @return A pointer to the texture
    std::shared_ptr<ITexture> SDLTextureFactory::LoadTexture(const std::string &texturePath)
    {
        std::string normalizedPath = Utils::NormalizePath(texturePath);

        auto textureBuffer = mCatalog->GetFile(normalizedPath);
        auto tex = IMG_LoadTexture_IO(mRenderer, SDL_IOFromConstMem(textureBuffer.data(), textureBuffer.size()), true);
        ASSERT("Failed loading texture from the catalog", tex != nullptr);
        if (tex == nullptr)
            throw EngineError(std::format("Texture {} was not loaded, but was found on the system", texturePath));

        SDL_SetTextureScaleMode(tex, SDL_ScaleMode::SDL_SCALEMODE_NEAREST); // SUPER IMPORTANT!
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

        return std::make_shared<SDLTexture>(tex);
    }

}

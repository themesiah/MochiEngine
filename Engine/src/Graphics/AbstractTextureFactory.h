#ifndef HDEF_ABSTRACTTEXTUREFACTORY
#define HDEF_ABSTRACTTEXTUREFACTORY

#include <memory>
#include <string>
#include <unordered_map>

namespace Mochi::Graphics
{
    class ITexture;
    /**
     * @brief This class must be inherited to generate textures of a compatible type with its own renderer.
     *
     *
     * Ownership:
     *
     * - It maintains a cache of the loaded textures as a map.
     *
     * - Clearing the cache is safe anytime, as all the created textures are shared pointers and will only be
     * destroyed when all references are lost.
     *
     *
     * Guarantees:
     *
     * - Valid textures will be only loaded once.
     *
     * - Cache will priorize loading from the file system.
     *
     * - Deleting a texture or the full cache is safe
     */
    class AbstractTextureFactory
    {
    protected:
        std::unordered_map<std::string, std::shared_ptr<ITexture>> mTexturesMap;
        /// @brief Implement this method to actually load the selected texture from memory onto a format
        /// that is compatible with the current renderer.
        /// @param texturePath The path of the texture in the virtual file system.
        /// @return A shared pointer to the texture.
        virtual std::shared_ptr<ITexture> LoadTexture(const std::string &texturePath) = 0;

    public:
        AbstractTextureFactory();
        virtual ~AbstractTextureFactory();
        /// @brief Gets the texture on the specified path on the file system.
        /// If the texture is in the cache it returns that one, otherwise it loads the texture from the system.
        /// @param texturePath The path or unique identifier.
        /// @return A shared pointer to the texture.
        std::shared_ptr<ITexture> GetTexture(const std::string &texturePath);
        /// @brief Deletes the texture cache from the factory.
        void DeleteCache();
        /// @brief Deletes a single texture from the cache.
        /// @param texturePath The texture to remove from the cache.
        void DeleteTexture(const std::string &texturePath);
    };
}

#endif
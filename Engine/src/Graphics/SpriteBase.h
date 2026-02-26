#ifndef HDEF_SPRITE
#define HDEF_SPRITE

#include <memory>
#include <vector>
#include <string>

#include "../Types/Types.hpp"
#include "RenderCommand.h"
#include "ITexture.h"
#include "../Types/Transform.hpp"

struct SDL_Texture;
namespace Mochi::Graphics
{
    class AbstractTextureFactory;
    /**
     * @brief This class contains all the necessary data to show textures on screen.
     *
     * Ownership:
     *
     * - The texture is shared between the sprite and the texture factory (and probably other sprites with the same texture identifier)
     *
     * - The transform is initialized in this class, but is shared and can be obtained and reused on other places, like colliders or debug gizmos.
     *
     *
     * Guarantees:
     *
     * - The render data obtained is always up to date with the texture and transform.
     *
     * - The texture has its color modified by the alpha value in the class.
     *
     * - It won't be shown if invisible.
     *
     * - Texture and transform will exist as long as the sprite exist (unless manually deleted somewhere).
     *
     *
     * Not responsible:
     *
     * - Sending its data to the renderer (user is responsible to send the RenderCommand to the renderer).
     */
    class SpriteBase
    {
    protected:
        std::shared_ptr<ITexture> mTexture;
        Rectf mSrcRect;
        uint16_t mZindex;
        bool mVisible;
        uint8_t mAlpha;
        Vector2f mSize;
        std::shared_ptr<Transform> mTransform;

    public:
        /// @brief Constructor that internally calls LoadTexture(AbstractTextureFactory *textureFactory, const std::string &filename)
        /// @param factory The texture factory from which to get the texture.
        /// @param path The path or unique identifier of the file in the file system.
        SpriteBase(AbstractTextureFactory *factory, const std::string &path);
        /// @brief This constructor only initializes a minimal part of the sprite. LoadTexture(AbstractTextureFactory *textureFactory, const std::string &filename)
        /// needs to be called for the sprite to be valid.
        SpriteBase();
        virtual ~SpriteBase();
        /// @brief Loads the texture and caches its size for future rendering.
        /// @param textureFactory The texture factory from which to get the texture.
        /// @param path The path or unique identifier of the file in the file system.
        void LoadTexture(AbstractTextureFactory *textureFactory, const std::string &path);
        /// @brief Gets the list of render commands that represent this sprite in a format compatible with the renderer.
        /// @return The list of render commands.
        virtual std::vector<RenderCommand> GetRenderData() const;
        /// @brief Overridable api to work with animated sprites.
        /// @param dt The delta time
        virtual void Update(const float &dt);
        void SetZIndex(const uint16_t &zIndex);
        uint16_t GetZIndex() const;
        bool IsVisible() const;
        void SetVisible(const bool &visible);
        uint8_t GetAlpha() const;
        void SetAlpha(const uint8_t &alpha);
        std::shared_ptr<Transform> GetTransform() const;
        void SetTransform(std::shared_ptr<Transform> tr);
        void SetSrcRect(const Rectf &src);
    };
}

#endif
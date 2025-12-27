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
        SpriteBase(AbstractTextureFactory *, const std::string &filename);
        SpriteBase();
        virtual ~SpriteBase();
        void LoadTexture(AbstractTextureFactory *textureFactory, const std::string &filename);
        virtual std::vector<RenderCommand> GetRenderData() const;
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
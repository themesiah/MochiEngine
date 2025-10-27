#ifndef HDEF_SPRITE
#define HDEF_SPRITE

#include <memory>
#include <vector>
#include <string>

#include "../Types/Types.hpp"
#include "RenderCommand.h"
#include "ITexture.h"

struct SDL_Texture;
namespace Mochi::Graphics
{
    class AbstractTextureFactory;
    class SpriteBase
    {
    private:
        std::shared_ptr<ITexture> mTexture;
        Rectf mSrcRect;
        float mScale;
        Vector2f mPosition;
        uint16_t mZindex;
        bool mVisible;
        uint8_t mAlpha;

    protected:
        Vector2f mSize;
        void SetSrcRect(const Rectf &srcRect);

    public:
        SpriteBase(AbstractTextureFactory *, const std::string &filename);
        SpriteBase();
        virtual ~SpriteBase();
        void LoadTexture(AbstractTextureFactory *textureFactory, const std::string &filename);
        virtual std::vector<RenderCommand> GetRenderData() const;
        virtual void Update(const float &dt);
        virtual void SetScale(const float &scale);
        float GetScale() const;
        void SetPosition(const Vector2f &position);
        Vector2f GetPosition() const;
        void Move(const Vector2f &movement);
        void SetZIndex(const uint16_t &zIndex);
        uint16_t GetZIndex() const;
        bool IsVisible() const;
        void SetVisible(const bool &visible);
        uint8_t GetAlpha() const;
        void SetAlpha(const uint8_t &alpha);
    };
}

#endif
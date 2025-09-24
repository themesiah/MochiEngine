#ifndef HDEF_SPRITE
#define HDEF_SPRITE

#include <memory>
#include <vector>
#include <string>

#include "../Types/Types.hpp"
#include "RenderCommand.h"

struct SDL_Texture;
namespace Mochi::Graphics
{
    class TextureFactory;
    class SpriteBase
    {
    private:
        std::shared_ptr<SDL_Texture> mTexture;
        Rectf mSrcRect;
        float mScale;
        Vector2f mPosition;

    protected:
        Vector2f mSize;
        void SetSrcRect(const Rectf &srcRect);

    public:
        SpriteBase(std::shared_ptr<TextureFactory>, const std::string &filename);
        SpriteBase();
        virtual ~SpriteBase();
        void LoadTexture(std::shared_ptr<TextureFactory> textureFactory, const std::string &filename);
        RenderCommand GetRenderData() const;
        void SetScale(const float &scale);
        float GetScale() const;
        void SetPosition(const Vector2f &position);
        Vector2f GetPosition() const;
    };
}

#endif
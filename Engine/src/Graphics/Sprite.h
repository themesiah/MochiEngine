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
    class Sprite
    {
    private:
        std::shared_ptr<SDL_Texture> mTexture;
        Rectf mDestRect;
        Rectf mSrcRect;
        float mScale;

    public:
        Sprite(std::shared_ptr<TextureFactory>, const std::string &filename);
        virtual ~Sprite();
        std::vector<RenderCommand> GetRenderData() const;
        void SetScale(const float &scale);
        float GetScale() const;
    };
}

#endif
#ifndef HDEF_SPRITE
#define HDEF_SPRITE

#include "../Entity/IRenderable.h"

#include <memory>
#include <vector>
#include <string>

#include "../Entity/IEntity.h"
#include "../Types/Types.hpp"

struct SDL_Texture;
namespace Mochi::Graphics
{
    class TextureFactory;
    class Sprite : public IRenderable, public IEntity
    {
    private:
        std::shared_ptr<SDL_Texture> mTexture;
        Rectf mDestRect;
        Rectf mSrcRect;
        float mScale;

    public:
        Sprite(std::shared_ptr<TextureFactory>, const std::string &filename);
        virtual ~Sprite();
        virtual std::vector<RenderCommand> GetRenderData() const;
        void SetScale(const float &scale);
        float GetScale() const;
    };
}

#endif
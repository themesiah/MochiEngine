#ifndef HDEF_SPRITESHEET
#define HDEF_SPRITESHEET

#include "../Entity/IEntity.h"
#include "../Entity/IRenderable.h"
#include "../Types/Types.hpp"

#include <memory>
#include <string>

struct SDL_Texture;
namespace Mochi::Graphics
{
    class AnimationsData;
    class TextureFactory;
    class AnimationFactory;
    class Spritesheet : public IRenderable, public IEntity
    {
    private:
        std::shared_ptr<SDL_Texture> mTexture;
        Rectf mDestRect;
        Rectf mSrcRect;
        float mScale;
        void ApplyFrameData();
        int mFrameIndex;

    protected:
        std::shared_ptr<AnimationsData> mAnimationsData;

    public:
        Spritesheet(std::shared_ptr<AnimationFactory> animationFactory, std::shared_ptr<TextureFactory> textureFactory, const std::string &animationPath, const int &startingFrame);
        ~Spritesheet();
        virtual std::vector<Graphics::RenderCommand> GetRenderData() const;
        void SetFrame(const int &frameIndex);
        int GetFrame() const;
        void SetScale(const float &scale);
        float GetScale() const;
        void SetPosition(const Vector2f &position);
        Vector2f GetPosition() const;
    };
}

#endif
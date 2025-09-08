#ifndef HDEF_ANIMATEDSPRITE
#define HDEF_ANIMATEDSPRITE

#include "../Entity/IAnimatable.h"
#include "../Entity/IRenderable.h"
#include "../Entity/IEntity.h"
#include "AnimationData.h"
#include "../Types/Types.hpp"

#include <string>
#include <memory>

namespace Mochi::Graphics
{
    class TextureFactory;
    class AnimationFactory;
    class AnimatedSprite : public IRenderable, public IAnimatable, public IEntity
    {
    protected:
        std::shared_ptr<AnimationsData> mAnimationsData;
        float mTimer;
        int mCurrentFrame;
        bool mForward;
        std::string mCurrentAnimation;

        std::shared_ptr<SDL_Texture> mTexture;
        Rectf mDestRect;
        Rectf mSrcRect;
        Vector2f mSize;
        float mScale;

    public:
        AnimatedSprite(std::shared_ptr<AnimationFactory>, std::shared_ptr<TextureFactory>, const std::string &animationPath, const std::string &mainAnimation);
        virtual ~AnimatedSprite();
        virtual RenderCommand GetRenderData() const;
        virtual void UpdateAnimation(const float &dt);
        void PlayAnimation(const std::string &animationName);
    };
}

#endif
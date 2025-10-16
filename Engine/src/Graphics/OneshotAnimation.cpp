#include "OneshotAnimation.h"
#include "AnimatedSprite.h"

#include "AbstractTextureFactory.h"
#include "IAnimationFactory.h"

namespace Mochi::Graphics
{
    OneshotAnimation::OneshotAnimation(IAnimationFactory *animationFactory, AbstractTextureFactory *textureFactory, const std::string &animationPath, const std::string &mainAnimation)
        : AnimatedSprite(animationFactory, textureFactory, animationPath, mainAnimation), mOnFinished()
    {
    }

    OneshotAnimation::~OneshotAnimation() {}

    void OneshotAnimation::Update(const float &dt)
    {
        AnimatedSprite::Update(dt);
        if (!mPlaying)
            return;
        if (mLoops > 0)
        {
            mOnFinished();
            StopAnimation();
        }
    }

    void OneshotAnimation::SetFinishCallback(std::function<void()> onFinishedCallback)
    {
        mOnFinished = onFinishedCallback;
    }
}
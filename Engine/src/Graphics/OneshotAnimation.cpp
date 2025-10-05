#include "OneshotAnimation.h"
#include "AnimatedSprite.h"

namespace Mochi::Graphics
{
    OneshotAnimation::OneshotAnimation(AnimationFactory *animationFactory, TextureFactory *textureFactory, const std::string &animationPath, const std::string &mainAnimation)
        : AnimatedSprite(animationFactory, textureFactory, animationPath, mainAnimation), mOnFinished()
    {
    }

    OneshotAnimation::~OneshotAnimation() {}

    void OneshotAnimation::UpdateAnimation(const float &dt)
    {
        AnimatedSprite::UpdateAnimation(dt);
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
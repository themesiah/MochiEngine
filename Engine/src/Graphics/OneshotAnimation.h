#ifndef HDEF_ONESHOTANIMATION
#define HDEF_ONESHOTANIMATION

#include "AnimatedSprite.h"

#include <functional>
#include <string>

namespace Mochi::Graphics
{
    class AbstractTextureFactory;
    class AnimationFactory;
    class OneshotAnimation : public AnimatedSprite
    {
    private:
        std::function<void()> mOnFinished;

    public:
        OneshotAnimation(AnimationFactory *, AbstractTextureFactory *, const std::string &animationPath, const std::string &mainAnimation);
        ~OneshotAnimation();
        virtual void UpdateAnimation(const float &dt) override;
        void SetFinishCallback(std::function<void()> onFinishedCallback);
    };
}

#endif
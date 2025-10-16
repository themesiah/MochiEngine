#ifndef HDEF_ONESHOTANIMATION
#define HDEF_ONESHOTANIMATION

#include "AnimatedSprite.h"

#include <functional>
#include <string>

namespace Mochi::Graphics
{
    class AbstractTextureFactory;
    class IAnimationFactory;
    class OneshotAnimation : public AnimatedSprite
    {
    private:
        std::function<void()> mOnFinished;

    public:
        OneshotAnimation(IAnimationFactory *, AbstractTextureFactory *, const std::string &animationPath, const std::string &mainAnimation);
        virtual ~OneshotAnimation();
        virtual void Update(const float &dt) override;
        void SetFinishCallback(std::function<void()> onFinishedCallback);
    };
}

#endif
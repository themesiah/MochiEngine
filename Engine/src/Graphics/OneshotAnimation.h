#ifndef HDEF_ONESHOTANIMATION
#define HDEF_ONESHOTANIMATION

#include "AnimatedSprite.h"

#include <functional>
#include <string>

namespace Mochi::Graphics
{
    class AbstractTextureFactory;
    class IAnimationFactory;
    /**
     * @brief OneshotAnimation is an AnimatedSprite limited to a single loop of an animation. It features a callback that is called when finishing that animation loop.
     * This makes the class useful for oneshot effects like explosions, and allows to destroy or hide them after finishing.
     *
     * Ownership:
     *
     * - It owns the callback assigned with SetFinishCallback.
     *
     * Guarantees:
     *
     * - The animation will be played once.
     *
     * - The callback will be invoked once, when the animation finishes.
     */
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
#ifndef HDEF_ANIMATEDSPRITE
#define HDEF_ANIMATEDSPRITE

#include "Spritesheet.h"
#include "AnimationData.h"
#include "../Types/Types.hpp"

#include <string>
#include <memory>
#include <vector>

namespace Mochi::Graphics
{
    class AbstractTextureFactory;
    class IAnimationFactory;
    class AnimatedSprite : public Spritesheet
    {
    protected:
        bool mPlaying;
        float mTimer;
        int mLoops;
        bool mForward;
        std::string mCurrentAnimation;

    public:
        AnimatedSprite(IAnimationFactory *, AbstractTextureFactory *, const std::string &animationPath, const std::string &mainAnimation);
        virtual ~AnimatedSprite();
        virtual void Update(const float &dt) override;
        virtual void PlayAnimation(const std::string &animationName);
        void StopAnimation();
        std::string GetCurrentAnimation() const;
    };
}

#endif
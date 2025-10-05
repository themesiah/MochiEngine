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
    class TextureFactory;
    class AnimationFactory;
    class AnimatedSprite : public Spritesheet
    {
    protected:
        float mTimer;
        int mLoops;
        bool mForward;
        std::string mCurrentAnimation;
        bool mPlaying;

    public:
        AnimatedSprite(AnimationFactory *, TextureFactory *, const std::string &animationPath, const std::string &mainAnimation);
        virtual ~AnimatedSprite();
        virtual void UpdateAnimation(const float &dt);
        virtual void PlayAnimation(const std::string &animationName);
        void StopAnimation();
        std::string GetCurrentAnimation() const;
    };
}

#endif
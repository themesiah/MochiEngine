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

    public:
        AnimatedSprite(std::shared_ptr<AnimationFactory>, std::shared_ptr<TextureFactory>, const std::string &animationPath, const std::string &mainAnimation);
        virtual ~AnimatedSprite();
        void UpdateAnimation(const float &dt);
        void PlayAnimation(const std::string &animationName);
        std::string GetCurrentAnimation() const;
    };
}

#endif
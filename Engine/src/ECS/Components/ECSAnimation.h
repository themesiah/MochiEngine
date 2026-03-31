#ifndef HDEF_ECSANIMATION
#define HDEF_ECSANIMATION

#include <string>

#include "../../Time/Timer.h"

namespace Mochi::ECS
{
    struct AnimationComponent
    {
        bool Playing = true;
        int CurrentFrame = 0;
        bool Forward = true;
        Time::Timer Timer{};
        std::string CurrentAnimation = "";
        int CurrentLoop = 0;
        std::string NewAnimation = "";

        AnimationComponent(const std::string &startingAnimation)
        {
            NewAnimation = startingAnimation;
        }

        void SetCurrentAnimation(const std::string &newAnimation)
        {
            NewAnimation = newAnimation;
        }
    };
}

#endif
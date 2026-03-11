#ifndef HDEF_ECSANIMATION
#define HDEF_ECSANIMATION

#include <string>

#include "../../Time/Timer.h"

namespace Mochi::ECS
{
    struct AnimationComponent
    {
        bool Playing;
        int CurrentFrame;
        bool Forward;
        Time::Timer Timer;
        std::string CurrentAnimation;
    };
}

#endif
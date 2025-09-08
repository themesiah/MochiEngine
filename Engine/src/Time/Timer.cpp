#include "Timer.h"

#include <stdexcept>

#include "../Exception.hpp"

namespace Mochi::Time
{
    Timer::Timer(const float &time) : mTargetTime(time), mTotalTime(0)
    {
        if (time < 0)
            throw EngineError("Time can not be less than 0");
    }

    float Timer::GetTime() const
    {
        return mTotalTime;
    }

    float Timer::GetRemainingTime() const
    {
        if (IsDone())
            return 0;
        return mTargetTime - mTotalTime;
    }

    bool Timer::IsDone() const
    {
        return mTotalTime >= mTargetTime;
    }

    void Timer::Tick(const float &dt)
    {
        if (dt < 0)
            throw EngineError("Delta time can not be less than 0");
        mTotalTime += dt;
    }
}

#include "Chrono.h"

#include <stdexcept>

#include "../Exception.hpp"

namespace Mochi::Time
{
    Chrono::Chrono() : mTotalTime(0)
    {
    }

    float Chrono::GetTime() const
    {
        return mTotalTime;
    }

    void Chrono::Tick(const float &dt)
    {
        if (dt < 0)
            throw EngineError("Delta time can not be less than 0");
        mTotalTime += dt;
    }
}

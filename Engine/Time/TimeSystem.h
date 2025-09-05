#ifndef HDEF_TIMESYSTEM
#define HDEF_TIMESYSTEM

#include "../Utils/Logger.h"
#include <format>

namespace Mochi::Time
{
    class TimeSystem
    {
    private:
        float mDeltaTime;
        float mUnscaledDeltaTime;
        float mGameTime;
        float mUnscaledGameTime;
        float mTimeScale;

        TimeSystem();

        float InternalGetDeltaTime() const;
        float InternalGetUnscaledDeltaTime() const;
        float InternalGetGameTime() const;
        float InternalGetUnscaledGameTime() const;
        void InternalSetTimeScale(const float &scale);

    public:
        static TimeSystem &GetInstance();
        TimeSystem(TimeSystem const &) = delete;
        void operator=(TimeSystem const &) = delete;
        ~TimeSystem();
        void Tick(const float &dt);

        static float GetDeltaTime();
        static float GetUnscaledDeltaTime();
        static float GetGameTime();
        static float GetUnscaledGameTime();
        static void SetTimeScale(const float &scale);
    };

}

#endif
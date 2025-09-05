#include "TimeSystem.h"

TimeSystem::TimeSystem() : mDeltaTime(0.f), mGameTime(0.f), mUnscaledGameTime(0.f), mTimeScale(1.f)
{
}

float TimeSystem::InternalGetDeltaTime() const { return mDeltaTime; }
float TimeSystem::InternalGetUnscaledDeltaTime() const { return mUnscaledDeltaTime; }
float TimeSystem::InternalGetGameTime() const { return mGameTime; }
float TimeSystem::InternalGetUnscaledGameTime() const { return mUnscaledGameTime; }
void TimeSystem::InternalSetTimeScale(const float &scale)
{
    if (scale < 0.f)
        throw std::runtime_error("Time scale time can't be negative");
    mTimeScale = scale;
}

TimeSystem &TimeSystem::GetInstance()
{
    static TimeSystem instance;
    return instance;
}
TimeSystem::~TimeSystem() {}
void TimeSystem::Tick(const float &dt)
{
    if (dt < 0.f)
        throw std::runtime_error("Delta time can't be negative");
    mDeltaTime = dt * mTimeScale;
    mUnscaledDeltaTime = dt;
    mGameTime += dt * mTimeScale;
    mUnscaledGameTime += dt;
}

float TimeSystem::GetDeltaTime() { return TimeSystem::GetInstance().InternalGetDeltaTime(); }
float TimeSystem::GetUnscaledDeltaTime() { return TimeSystem::GetInstance().InternalGetUnscaledDeltaTime(); }
float TimeSystem::GetGameTime() { return TimeSystem::GetInstance().InternalGetGameTime(); }
float TimeSystem::GetUnscaledGameTime() { return TimeSystem::GetInstance().InternalGetUnscaledGameTime(); }
void TimeSystem::SetTimeScale(const float &scale) { TimeSystem::GetInstance().InternalSetTimeScale(scale); }
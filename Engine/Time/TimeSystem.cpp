#include "TimeSystem.h"

TimeSystem::TimeSystem() : mDeltaTime(0.f), mGameTime(0.f), mUnscaledGameTime(0.f), mTimeScale(1.f)
{
}

float TimeSystem::InternalGetDeltaTime() const { return mDeltaTime * mTimeScale; }
float TimeSystem::InternalGetUnscaledDeltaTime() const { return mDeltaTime; }
float TimeSystem::InternalGetGameTime() const { return mGameTime; }
float TimeSystem::InternalGetUnscaledGameTime() const { return mUnscaledGameTime; }
void TimeSystem::InternalSetTimeScale(const float &scale) { mTimeScale = scale; }

TimeSystem &TimeSystem::GetInstance()
{
    static TimeSystem instance;
    return instance;
}
TimeSystem::~TimeSystem() {}
void TimeSystem::Tick(const float &dt)
{
    mDeltaTime = dt;
    mGameTime += dt * mTimeScale;
    mUnscaledGameTime += dt;
    // LOG_INFO(std::format("Ticking with dt {} and now delta time is {}", dt, mDeltaTime));
}

float TimeSystem::GetDeltaTime() { return TimeSystem::GetInstance().InternalGetDeltaTime(); }
float TimeSystem::GetUnscaledDeltaTime() { return TimeSystem::GetInstance().InternalGetUnscaledDeltaTime(); }
float TimeSystem::GetGameTime() { return TimeSystem::GetInstance().InternalGetGameTime(); }
float TimeSystem::GetUnscaledGameTime() { return TimeSystem::GetInstance().InternalGetUnscaledGameTime(); }
void TimeSystem::SetTimeScale(const float &scale) { TimeSystem::GetInstance().InternalSetTimeScale(scale); }
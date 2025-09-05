// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Time/TimeSystem.h"
#include "Time/Timer.h"
#include "Time/Chrono.h"

TEST_CASE("Time system")
{
    CHECK_EQ(TimeSystem::GetGameTime(), 0);
    CHECK_EQ(TimeSystem::GetDeltaTime(), 0);
    CHECK_EQ(TimeSystem::GetUnscaledGameTime(), 0);
    CHECK_EQ(TimeSystem::GetUnscaledDeltaTime(), 0);

    TimeSystem::GetInstance().Tick(0);
    CHECK_EQ(TimeSystem::GetGameTime(), 0);
    CHECK_EQ(TimeSystem::GetDeltaTime(), 0);
    CHECK_EQ(TimeSystem::GetUnscaledGameTime(), 0);
    CHECK_EQ(TimeSystem::GetUnscaledDeltaTime(), 0);

    TimeSystem::GetInstance().Tick(1);
    CHECK_EQ(TimeSystem::GetGameTime(), 1);
    CHECK_EQ(TimeSystem::GetDeltaTime(), 1);
    CHECK_EQ(TimeSystem::GetUnscaledGameTime(), 1);
    CHECK_EQ(TimeSystem::GetUnscaledDeltaTime(), 1);

    TimeSystem::GetInstance().Tick(1);
    CHECK_EQ(TimeSystem::GetGameTime(), 2);
    CHECK_EQ(TimeSystem::GetDeltaTime(), 1);
    CHECK_EQ(TimeSystem::GetUnscaledGameTime(), 2);
    CHECK_EQ(TimeSystem::GetUnscaledDeltaTime(), 1);

    TimeSystem::SetTimeScale(2);
    TimeSystem::GetInstance().Tick(1);
    CHECK_EQ(TimeSystem::GetGameTime(), 4);
    CHECK_EQ(TimeSystem::GetDeltaTime(), 2);
    CHECK_EQ(TimeSystem::GetUnscaledGameTime(), 3);
    CHECK_EQ(TimeSystem::GetUnscaledDeltaTime(), 1);

    TimeSystem::SetTimeScale(0.5f);
    TimeSystem::GetInstance().Tick(2);
    CHECK_EQ(TimeSystem::GetGameTime(), 5);
    CHECK_EQ(TimeSystem::GetDeltaTime(), 1);
    CHECK_EQ(TimeSystem::GetUnscaledGameTime(), 5);
    CHECK_EQ(TimeSystem::GetUnscaledDeltaTime(), 2);

    TimeSystem::SetTimeScale(0.f);
    TimeSystem::GetInstance().Tick(1);
    CHECK_EQ(TimeSystem::GetGameTime(), 5);
    CHECK_EQ(TimeSystem::GetDeltaTime(), 0);
    CHECK_EQ(TimeSystem::GetUnscaledGameTime(), 6);
    CHECK_EQ(TimeSystem::GetUnscaledDeltaTime(), 1);

    CHECK_THROWS(TimeSystem::SetTimeScale(-1));
    CHECK_THROWS(TimeSystem::GetInstance().Tick(-1));
}

TEST_CASE("Timer")
{
    Timer t(5);
    CHECK_FALSE(t.IsDone());
    t.Tick(1);
    CHECK_FALSE(t.IsDone());
    CHECK_EQ(t.GetRemainingTime(), 4);
    t.Tick(1);
    CHECK_FALSE(t.IsDone());
    CHECK_EQ(t.GetRemainingTime(), 3);
    t.Tick(2);
    CHECK_FALSE(t.IsDone());
    CHECK_THROWS(t.Tick(-1));
    CHECK_EQ(t.GetRemainingTime(), 1);
    t.Tick(0);
    CHECK_FALSE(t.IsDone());
    t.Tick(1);
    CHECK(t.IsDone());
    CHECK_EQ(t.GetRemainingTime(), 0);
    t.Tick(1);
    CHECK_EQ(t.GetRemainingTime(), 0);
    CHECK(t.IsDone());
}

TEST_CASE("Chrono")
{
    Mochi::Time::Chrono chrono;
    chrono.Tick(1.5f);
    CHECK_EQ(chrono.GetTime(), 1.5f);
}
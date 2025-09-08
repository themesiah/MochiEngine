#ifndef HDEF_TIMER
#define HDEF_TIMER

namespace Mochi::Time
{
    class Timer
    {
    private:
        float mTargetTime;
        float mTotalTime;

    public:
        float GetTime() const;
        float GetRemainingTime() const;
        bool IsDone() const;
        Timer(const float &time);
        void Tick(const float &dt);
    };
}

#endif
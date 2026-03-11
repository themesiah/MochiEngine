#ifndef HDEF_TIMER
#define HDEF_TIMER

namespace Mochi::Time
{
    /**
     * @brief Utility class to count the pass of time. If ticked each frame, IsDone will return true when the time parameter in the construction has passed.
     * It will also show the passed time like a Chrono in GetTime and the difference between the construction time and GetTime with GetRemainingTime.
     */
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
        Timer();
        void Tick(const float &dt);
    };
}

#endif
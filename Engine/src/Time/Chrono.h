#ifndef HDEF_CHRONO
#define HDEF_CHRONO

namespace Mochi::Time
{
    /**
     * @brief Utility class to count the pass of time.
     * If ticked each frame, GetTime will return the passed time since the Chrono was created.
     */
    class Chrono
    {
    private:
        float mTotalTime;

    public:
        float GetTime() const;
        Chrono();
        void Tick(const float &dt);
    };
}

#endif
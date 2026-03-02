#ifndef HDEF_TIMESYSTEM
#define HDEF_TIMESYSTEM

#include "../Utils/Logger.h"
#include <format>

namespace Mochi::Time
{
    /**
     * @brief Class that provides several time values useful for game development.
     *
     * This class has to be updated calling Tick with the real delta time passed since the last frame.
     * This way, TimeSystem stores it, applies time scale if needed and updated the total time passed since the application started.
     *
     * Guarantees:
     *
     * - Access everywhere in the engine to all time values provided by the class via static methods.
     *
     * - A time scale greater than 0.
     *
     * - No negative real delta time can be used to update.
     *
     * - Update is done automatically by the Engine at each frame.
     *
     *
     * Thread safety:
     *
     * - Accessing time is thread safe.
     */
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
        float InternalGetTimeScale() const;

    public:
        static TimeSystem &GetInstance();
        TimeSystem(TimeSystem const &) = delete;
        void operator=(TimeSystem const &) = delete;
        ~TimeSystem();
        void Tick(const float &dt);

        /// @brief Gets the last delta time, scaled by the time scale
        /// @return Time passed since the last frame, scaled
        static float GetDeltaTime();
        /// @brief Gets the last delta time without scale
        /// @return Real time passed since the last frame
        static float GetUnscaledDeltaTime();
        /// @brief Gets the time passed since opening the application, scaled
        /// @return Time passed since opening the application, scaled
        static float GetGameTime();
        /// @brief Gets the time passed since opening the application
        /// @return Time passed since opening the application
        static float GetUnscaledGameTime();
        /// @brief Sets a time scale
        /// @remark Scale has to be greater or equal than 0, (useful to pause if 0)
        /// @param scale The new time scale, greater or equal than 0
        static void SetTimeScale(const float &scale);
        /// @brief Gets the current time scale
        /// @return Time scale
        static float GetTimeScale();
    };

}

#endif
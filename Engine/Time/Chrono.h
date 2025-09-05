#ifndef HDEF_CHRONO
#define HDEF_CHRONO

namespace Mochi
{
    namespace Time
    {
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
}
#endif
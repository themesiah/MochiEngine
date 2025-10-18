#ifndef HDEF_PROFILER
#define HDEF_PROFILER

#include <chrono>
#include <string>
#include <unordered_map>
#include <iostream>
#include <sstream>

namespace Mochi::Debug
{
    class FrameProfiler
    {
    public:
        void BeginSection(const std::string &name)
        {
            mSections[name].start = std::chrono::high_resolution_clock::now();
        }

        void EndSection(const std::string &name)
        {
            auto end = std::chrono::high_resolution_clock::now();
            auto &s = mSections[name];
            s.accum += std::chrono::duration<double, std::milli>(end - s.start).count();
        }

        void NewFrame()
        {
            for (auto &[_, s] : mSections)
                s.accum = 0.0;
        }

        std::string Report() const
        {
            double total = 0.0;
            for (auto &[name, s] : mSections)
                total += s.accum;

            std::stringstream ss;
            ss << "START OF FRAME REPORT\n";
            ss << "Frame " << total << " ms\n";
            for (auto &[name, s] : mSections)
                ss << name << ": " << s.accum << "ms\n";
            ss << "END OF FRAME REPORT\n";
            return ss.str();
        }

    private:
        struct Section
        {
            std::chrono::high_resolution_clock::time_point start;
            double accum = 0.0;
        };
        std::unordered_map<std::string, Section> mSections;
    };
}

#endif
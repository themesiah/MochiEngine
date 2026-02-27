#ifndef HDEF_IAUDIOMANAGER
#define HDEF_IAUDIOMANAGER

#include <string>

namespace Mochi::Audio
{
    /**
     * @brief Interface for audio management, including loading and unloading audio, playing it, changing volume and other parameters.
     *
     * The implementations are meant to wrap some existing audio system to be used the same within the engine.
     */
    class IAudioManager
    {
    public:
        virtual ~IAudioManager() {};
        virtual void Update(const float &dt) = 0;
        virtual void LoadAudio(const std::string &path) = 0;
        virtual void UnloadAudio(const std::string &path) = 0;
        virtual void PlayBGM(const std::string &audioName) = 0;
        virtual void PauseBGM() = 0;
        virtual void StopBGM() = 0;
        virtual void ResumeBGM() = 0;
        virtual void SkipToTimelinePosition(const int &ms) = 0;
        virtual void PlayOneShot(const std::string &audioName) = 0;
        virtual void SetParameter(const std::string &parameterName, const float &value) = 0;
        virtual void SetMixerVolume(const std::string &mixerGroupName, const float &value) = 0;
    };
}

#endif
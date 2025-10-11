#ifndef HDEF_DUMMYAUDIOMANAGER
#define HDEF_DUMMYAUDIOMANAGER

#include "IAudioManager.h"

namespace Mochi::Audio
{
    class DummyAudioManager : public IAudioManager
    {
    public:
        virtual ~DummyAudioManager() {};
        virtual void Update(const float &dt) override {};
        virtual void LoadAudio(const std::string &path) override {};
        virtual void UnloadAudio(const std::string &path) override {};
        virtual void PlayBGM(const std::string &audioName) override {};
        virtual void PauseBGM() override {};
        virtual void StopBGM() override {};
        virtual void ResumeBGM() override {};
        virtual void SkipToTimelinePosition(const int &ms) override {};
        virtual void PlayOneShot(const std::string &audioName) override {};
        virtual void SetParameter(const std::string &parameterName, const float &value) override {};
        virtual void SetMixerVolume(const std::string &mixerGroupName, const float &value) override {};
    };
}

#endif
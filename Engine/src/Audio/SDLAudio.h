#ifndef USE_FMOD
#ifndef HDEF_SDLAUDIO
#define HDEF_SDLAUDIO

#include "IAudioManager.h"

#include <memory>
#include <unordered_map>

namespace Mochi::FS
{
    class PackCatalog;
}
struct MIX_Mixer;
struct MIX_Group;
struct MIX_Track;
struct MIX_Audio;
namespace Mochi::Audio
{
    class SDLAudio : public IAudioManager
    {
    private:
        FS::PackCatalog *mCatalog;
        std::unique_ptr<MIX_Mixer, void (*)(MIX_Mixer *)> mMixer;
        std::unique_ptr<MIX_Track, void (*)(MIX_Track *)> mBgmTrack;
        std::unordered_map<std::string, MIX_Group *> mGroups;
        std::unordered_map<std::string, MIX_Audio *> mAudioCache;

    public:
        SDLAudio(FS::PackCatalog *catalog);
        virtual ~SDLAudio();
        virtual void Update(const float &dt) override;
        virtual void LoadAudio(const std::string &path) override;
        virtual void UnloadAudio(const std::string &path) override;
        virtual void PlayBGM(const std::string &audioName) override;
        virtual void PauseBGM() override;
        virtual void StopBGM() override;
        virtual void ResumeBGM() override;
        virtual void SkipToTimelinePosition(const int &ms) override;
        virtual void PlayOneShot(const std::string &audioName) override;
        virtual void SetParameter(const std::string &parameterName, const float &value) override;
        virtual void SetMixerVolume(const std::string &mixerGroupName, const float &value) override;
    };
}

#endif
#endif
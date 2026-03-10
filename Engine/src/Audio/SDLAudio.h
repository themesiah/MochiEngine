#if !defined(USE_FMOD) || defined(GENERATE_DOCS)
#ifndef HDEF_SDLAUDIO
#define HDEF_SDLAUDIO

#include "IAudioManager.h"

#include <nlohmann/json_fwd.hpp>
#include <SDL3_mixer/SDL_mixer.h>

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

#include "../Utils/SDLUtils.h"

namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Audio
{
    SDL_DELETER(MIX_Track, MIX_DestroyTrack)
    SDL_DELETER(MIX_Audio, MIX_DestroyAudio)
    SDL_DELETER(MIX_Mixer, MIX_DestroyMixer)
    using TrackPtr = std::unique_ptr<MIX_Track, MIX_TrackDeleter>;
    using MixerPtr = std::unique_ptr<MIX_Mixer, MIX_MixerDeleter>;
    using AudioPtr = std::unique_ptr<MIX_Audio, MIX_AudioDeleter>;
    class SDLAudio : public IAudioManager
    {
    private:
        enum AudioEntryType
        {
            SFX,
            BGM
        };
        struct AudioEntry
        {
            std::string Name;
            std::string Path;
            AudioEntryType Type;
        };

        FS::PackCatalog *mCatalog;
        MixerPtr mMixer;
        TrackPtr mBgmTrack;
        std::vector<TrackPtr> mTrackPool;
        std::unordered_map<std::string, AudioPtr> mAudioCache;

        std::unordered_map<std::string, AudioEntry> mEntries;

        void LoadAudioEntry(const nlohmann::json &j);
        void UnloadAudioEntry(const std::string &entry);
        MIX_Track *FindFreeTrack();
        bool Preload(const std::string &path);

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
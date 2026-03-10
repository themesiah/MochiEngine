#if !defined(USE_FMOD) || defined(GENERATE_DOCS)
#include "SDLAudio.h"

#include <SDL3_mixer/SDL_mixer.h>
#include <nlohmann/json.hpp>

#include "../Packer/PackCatalog.h"
#include "../Exception.hpp"
#include "../Utils/Logger.h"
#include "../Utils/StringUtils.h"
#include "../Utils/MathUtils.h"

namespace Mochi::Audio
{
    inline constexpr int MAX_TRACKS = 20;
    inline constexpr float SDL_MIXER_MAX_VOLUME = 1.2f;

    SDLAudio::SDLAudio(FS::PackCatalog *catalog)
        : mMixer({nullptr}),
          mBgmTrack({nullptr}),
          mEntries(),
          mTrackPool()
    {
        mCatalog = catalog;

        if (!MIX_Init())
        {
            throw SystemInitializationError("Mixer", SDL_GetError());
        }

        MIX_Mixer *mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
        if (mixer == nullptr)
        {
            throw SystemInitializationError("Mixer", "Failed to create a mixer");
        }
        mMixer.reset(mixer);

        MIX_Track *bgmTrack = MIX_CreateTrack(mMixer.get());
        mBgmTrack.reset(bgmTrack);

        MIX_SetTrackLoops(mBgmTrack.get(), -1);

        mTrackPool.resize(MAX_TRACKS);
        for (int i = 0; i < MAX_TRACKS; ++i)
        {
            // mTrackPool[i] = std::make_unique<MIX_Track>(MIX_CreateTrack(mMixer.get()), MIX_DestroyTrack);
            auto track = MIX_CreateTrack(mMixer.get());
            mTrackPool[i] = TrackPtr(track);
            MIX_SetTrackLoops(mTrackPool[i].get(), 1);
        }

        LOG_OK("SDL Mixer initialized");
    }

    SDLAudio::~SDLAudio()
    {
        mMixer.reset();
        MIX_Quit();
    }

    void SDLAudio::Update(const float &dt)
    {
    }

    void SDLAudio::LoadAudio(const std::string &path)
    {
        std::string normalizedPath = Utils::NormalizePath(path);
        auto jsonContent = mCatalog->GetFile(normalizedPath);
        try
        {
            nlohmann::json data = nlohmann::json::parse(jsonContent);
            auto entries = data.at("Entries");

            if (!entries.empty())
            {
                for (size_t i = 0; i < entries.size(); ++i)
                {
                    nlohmann::json entry = entries[i];
                    LoadAudioEntry(entry);
                }
            }
        }
        catch (const std::exception &e)
        {
            throw EngineError(e.what());
        }
    }

    void SDLAudio::LoadAudioEntry(const nlohmann::json &j)
    {
        std::string path = j.at("Path");
        std::string name = j.at("Name");
        bool preload = j.at("Preload");

        if (mEntries.find(name) != mEntries.end())
        {
            UnloadAudioEntry(name);
        }

        AudioEntry entry{
            .Name = name,
            .Path = path};

        mEntries[name] = entry;

        if (preload)
        {
            Preload(name);
        }
    }

    void SDLAudio::UnloadAudioEntry(const std::string &entry)
    {
        if (mAudioCache.find(entry) != mAudioCache.end())
        {
            // MIX_Audio are unique pointers. Erasing them destroys them using MIX_DestroyAudio.
            mAudioCache.erase(entry);
        }
        mEntries.erase(entry);
    }

    MIX_Track *SDLAudio::FindFreeTrack()
    {
        for (size_t i = 0; i < mTrackPool.size(); ++i)
        {
            if (!MIX_TrackPlaying(mTrackPool[i].get()))
            {
                return mTrackPool[i].get();
            }
        }
        return mTrackPool[0].get();
    }

    bool SDLAudio::Preload(const std::string &name)
    {
        if (mAudioCache.find(name) != mAudioCache.end())
        {
            return true;
        }

        if (mEntries.find(name) == mEntries.end())
        {
            LOG_ERROR(std::format("Audio with name {} was not loaded in the json file", name));
            return false;
        }

        auto entry = mEntries[name];
        auto path = Utils::NormalizePath(entry.Path);
        auto audioBuffer = mCatalog->GetFile(path);
        MIX_Audio *audio = MIX_LoadAudio_IO(mMixer.get(), SDL_IOFromConstMem(audioBuffer.data(), audioBuffer.size()), false, true);

        if (audio == nullptr)
        {
            LOG_ERROR(std::format("Cant load audio on path {}", path));
            return false;
        }
        else
        {
            // mAudioCache[name] = std::make_unique<MIX_Audio>(audio, MIX_DestroyAudio);
            mAudioCache[name] = AudioPtr(audio);
            return true;
        }
    }

    void SDLAudio::UnloadAudio(const std::string &path)
    {
        std::string normalizedPath = Utils::NormalizePath(path);
        auto jsonContent = mCatalog->GetFile(normalizedPath);
        try
        {
            nlohmann::json data = nlohmann::json::parse(jsonContent);
            auto entries = data.at("Entries");

            if (!entries.empty())
            {
                for (size_t i = 0; i < entries.size(); ++i)
                {
                    nlohmann::json entry = entries[i];
                    UnloadAudioEntry(entry.at("Name"));
                }
            }
        }
        catch (const std::exception &e)
        {
            throw EngineError(e.what());
        }
    }

    void SDLAudio::PlayBGM(const std::string &audioName)
    {
        Preload(audioName);

        if (!MIX_SetTrackAudio(mBgmTrack.get(), mAudioCache[audioName].get()))
        {
            LOG_ERROR("Can't set BGM track audio");
        }

        if (!MIX_PlayTrack(mBgmTrack.get(), NULL))
        {
            LOG_ERROR("Can't play music track");
        }
    }

    void SDLAudio::PauseBGM()
    {
        MIX_PauseTrack(mBgmTrack.get());
    }

    void SDLAudio::StopBGM()
    {
        MIX_StopTrack(mBgmTrack.get(), 0);
    }

    void SDLAudio::ResumeBGM()
    {
        MIX_ResumeTrack(mBgmTrack.get());
    }

    void SDLAudio::SkipToTimelinePosition(const int &ms)
    {
        MIX_SetTrackPlaybackPosition(mBgmTrack.get(), MIX_TrackMSToFrames(mBgmTrack.get(), ms));
    }

    void SDLAudio::PlayOneShot(const std::string &audioName)
    {
        Preload(audioName);
        auto track = FindFreeTrack();

        if (!MIX_SetTrackAudio(track, mAudioCache[audioName].get()))
        {
            LOG_ERROR("Can't set BGM track audio");
        }

        if (!MIX_PlayTrack(track, NULL))
        {
            LOG_ERROR("Can't play music track");
        }
    }

    void SDLAudio::SetParameter(const std::string &parameterName, const float &value)
    {
    }

    void SDLAudio::SetMixerVolume(const std::string &mixerGroupName, const float &value)
    {
        float newVolume = Math::Clamp(value, 0.0f, SDL_MIXER_MAX_VOLUME);

        if (mixerGroupName.compare("SFX") == 0)
        {
            for (size_t i = 0; i < mTrackPool.size(); ++i)
            {
                MIX_SetTrackGain(mTrackPool[i].get(), newVolume);
            }
        }
        else if (mixerGroupName.compare("BGM") == 0)
        {
            MIX_SetTrackGain(mBgmTrack.get(), newVolume);
        }
        else if (mixerGroupName.compare("") == 0)
        {
            MIX_SetMixerGain(mMixer.get(), newVolume);
        }
    }
}

#endif
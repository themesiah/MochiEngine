#ifndef USE_FMOD
#include "SDLAudio.h"

#include <SDL3_mixer/SDL_mixer.h>

#include "../Packer/PackCatalog.h"
#include "../Exception.hpp"
#include "../Utils/Logger.h"
#include "../Utils/StringUtils.h"

namespace Mochi::Audio
{
    SDLAudio::SDLAudio(FS::PackCatalog *catalog)
        : mMixer({nullptr, MIX_DestroyMixer}),
          mBgmTrack({nullptr, MIX_DestroyTrack}),
          mGroups()
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

        mGroups["SFX"] = MIX_CreateGroup(mMixer.get());
        mGroups["BGM"] = MIX_CreateGroup(mMixer.get());

        MIX_Track *bgmTrack = MIX_CreateTrack(mMixer.get());
        mBgmTrack.reset(bgmTrack);

        MIX_SetTrackGroup(mBgmTrack.get(), mGroups["BGM"]);

        LOG_OK("SDL Mixer initialized");
    }

    SDLAudio::~SDLAudio()
    {
        mMixer.reset();
    }

    void SDLAudio::Update(const float &dt)
    {
    }

    void SDLAudio::LoadAudio(const std::string &path)
    {
        std::string normalizedPath = Utils::NormalizePath(path);
        if (mAudioCache.find(normalizedPath) != mAudioCache.end())
        {
            LOG_WARNING(std::format("Audio in path {} was already loaded.", normalizedPath));
            return;
        }

        auto audioBuffer = mCatalog->GetFile(normalizedPath);
        MIX_Audio *audio = MIX_LoadAudio_IO(mMixer.get(), SDL_IOFromConstMem(audioBuffer.data(), audioBuffer.size()), false, true);
        if (audio == nullptr)
        {
            LOG_ERROR("Cant load audio");
        }
        mAudioCache[normalizedPath] = audio;
    }

    void SDLAudio::UnloadAudio(const std::string &path)
    {
        std::string normalizedPath = Utils::NormalizePath(path);
        if (mAudioCache.find(normalizedPath) == mAudioCache.end())
        {
            LOG_WARNING(std::format("Audio on path {} does not exist. Can't unload.", normalizedPath));
            return;
        }

        auto audio = mAudioCache[normalizedPath];
        MIX_DestroyAudio(audio);
        mAudioCache.erase(normalizedPath);
    }

    void SDLAudio::PlayBGM(const std::string &audioName)
    {
        std::string normalizedPath = Utils::NormalizePath(audioName);
        if (mAudioCache.find(normalizedPath) == mAudioCache.end())
        {
            LoadAudio(audioName);
        }

        if (!MIX_SetTrackAudio(mBgmTrack.get(), mAudioCache[normalizedPath]))
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
    }

    void SDLAudio::StopBGM()
    {
    }

    void SDLAudio::ResumeBGM()
    {
    }

    void SDLAudio::SkipToTimelinePosition(const int &ms)
    {
    }

    void SDLAudio::PlayOneShot(const std::string &audioName)
    {
    }

    void SDLAudio::SetParameter(const std::string &parameterName, const float &value)
    {
    }

    void SDLAudio::SetMixerVolume(const std::string &mixerGroupName, const float &value)
    {
    }
}

#endif
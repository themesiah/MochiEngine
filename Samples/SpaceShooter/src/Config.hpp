#ifndef HDEF_CONFIG
#define HDEF_CONFIG

#include "Audio/IAudioManager.h"
#include "Input/IActionManager.h"
#include "Utils/MathUtils.h"

namespace Mochi::Shooter
{
    class Config
    {
    private:
        Audio::IAudioManager *mAudioManager;
        float mVolume;

    public:
        Config(Audio::IAudioManager *audioManager) : mVolume(0.5f)
        {
            mAudioManager = audioManager;
        }
        virtual ~Config() {}
        void Init()
        {
            mAudioManager->SetMixerVolume("", mVolume);
        }
        void Update(const float &dt, Input::IActionManager *actionManager)
        {
            if (actionManager->Performed("Debug1"))
            {
                mVolume -= 0.1f;
                mVolume = Math::Max(mVolume, 0.0f);
                mAudioManager->SetMixerVolume("", mVolume);
            }
            if (actionManager->Performed("Debug2"))
            {
                mVolume += 0.1f;
                mVolume = Math::Min(mVolume, 1.0f);
                mAudioManager->SetMixerVolume("", mVolume);
            }
        }
    };
}

#endif
#ifndef HDEF_FMODWRAPPER
#define HDEF_FMODWRAPPER

#include "IAudioManager.h"

#include <fmod_common.h>
#include <fmod_studio_common.h>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <sol/sol.hpp>

struct FMOD_STUDIO_SYSTEM;
struct FMOD_STUDIO_BANK;
struct FMOD_STUDIO_EVENTDESCRIPTION;
struct FMOD_STUDIO_EVENTINSTANCE;

namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Scripting
{
    class ScriptingManager;
}
namespace Mochi::Audio
{
    struct FMODMarkerEvent
    {
        const char *MarkerName;
        int MarkerPosition;
    };
    class FMODWrapper : public IAudioManager
    {
    private:
        struct FMOD_Bank_Pair
        {
            FMOD_STUDIO_BANK *bank;
            FMOD_STUDIO_BANK *stringsBank;
        };

        FMOD_STUDIO_SYSTEM *mFmodSystem;
        FMOD_STUDIO_EVENTDESCRIPTION *mBgmEventDescription;
        FMOD_STUDIO_EVENTINSTANCE *mBgmEventInstance;
        std::vector<FMOD_Bank_Pair> mBankPairs;
        std::unordered_map<std::string, FMOD_Bank_Pair> mBankPairsMap;
        static FMOD_RESULT F_CALL EventCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters);
        void OnEventCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters);
        FS::PackCatalog *mCatalog;
        Scripting::ScriptingManager *mScripting;
        std::unordered_map<std::string, FMOD_STUDIO_EVENTDESCRIPTION *> mSoundCache;
        std::unordered_map<std::string, FMOD_STUDIO_BUS *> mBusesCache;

        void LoadBank(const std::string &bankName);
        void UnloadBank(const std::string &bankName);
        void SetBusVolume(const std::string &busGroupName, const float &value);

        std::vector<FMODMarkerEvent> mEventQueue;

    public:
        FMODWrapper(FS::PackCatalog *, Scripting::ScriptingManager *);
        virtual ~FMODWrapper();
        virtual void Update(const float &dt) override;
        virtual void LoadAudio(const std::string &path) override { LoadBank(path); }
        virtual void UnloadAudio(const std::string &path) override { UnloadBank(path); };
        virtual void PlayBGM(const std::string &eventName) override;
        virtual void PauseBGM() override;
        virtual void ResumeBGM() override;
        virtual void StopBGM() override;
        virtual void SkipToTimelinePosition(const int &ms) override;
        virtual void PlayOneShot(const std::string &eventName) override;
        void ClearSoundCache() { mSoundCache.clear(); }
        void ClearBusCache() { mBusesCache.clear(); }
        virtual void SetParameter(const std::string &parameterName, const float &value) override;
        virtual void SetMixerVolume(const std::string &busGroupName, const float &value) override { SetBusVolume(busGroupName, value); };
    };
}

#endif
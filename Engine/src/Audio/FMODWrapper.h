#ifndef HDEF_FMODWRAPPER
#define HDEF_FMODWRAPPER

#include <fmod_common.h>
#include <fmod_studio_common.h>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

struct FMOD_STUDIO_SYSTEM;
struct FMOD_STUDIO_BANK;
struct FMOD_STUDIO_EVENTDESCRIPTION;
struct FMOD_STUDIO_EVENTINSTANCE;

namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Audio
{
    class FMODWrapper
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
        std::shared_ptr<FS::PackCatalog> mCatalog;
        std::unordered_map<std::string, FMOD_STUDIO_EVENTDESCRIPTION *> mSoundCache;
        std::unordered_map<std::string, FMOD_STUDIO_BUS *> mBusesCache;

    public:
        FMODWrapper(std::shared_ptr<FS::PackCatalog>);
        ~FMODWrapper();
        void Update() const;
        void LoadBank(const std::string &bankName);
        void UnloadBank(const std::string &bankName);
        void PlayBGM(const std::string &eventName);
        void PauseBGM();
        void ResumeBGM();
        void StopBGM();
        void SkipToTimelinePosition(const int &ms);
        void PlayOneShot(const std::string &eventName);
        void ClearSoundCache() { mSoundCache.clear(); }
        void ClearBusCache() { mBusesCache.clear(); }
        void SetParameter(const std::string &parameterName, float value);
        void SetBusVolume(const std::string &busGroupName, float value);
    };
}

#endif
#ifndef HDEF_FMODWRAPPER
#define HDEF_FMODWRAPPER

#include <fmod_common.h>
#include <fmod_studio_common.h>
#include <vector>
#include <string>

struct FMOD_STUDIO_SYSTEM;
struct FMOD_STUDIO_BANK;
struct FMOD_STUDIO_EVENTDESCRIPTION;
struct FMOD_STUDIO_EVENTINSTANCE;
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
    void PrintFMODError(FMOD_RESULT result) const;
    static FMOD_RESULT F_CALL EventCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters);
    void OnEventCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters);

public:
    FMODWrapper();
    ~FMODWrapper();
    FMOD_RESULT Init();
    FMOD_RESULT Update() const;
    FMOD_RESULT LoadBank(const std::string &bankName);
    FMOD_RESULT PlayBGM(const std::string &eventName);
    FMOD_RESULT PauseBGM();
    FMOD_RESULT ResumeBGM();
    FMOD_RESULT StopBGM();
};

#endif
#include "FMODWrapper.h"

#include <iostream>
#include <format>
#include <fmod_studio.h>
#include <fmod.h>
#include <fmod_common.h>
#include <fmod_errors.h>

FMODWrapper::FMODWrapper()
{
    mFmodSystem = NULL;
    mBgmEventDescription = NULL;
    mBgmEventInstance = NULL;
}

FMOD_RESULT FMODWrapper::Init()
{
    FMOD_RESULT result;

    result = FMOD_Studio_System_Create(&mFmodSystem, FMOD_VERSION);
    if (result != FMOD_OK)
    {
        std::cout << "Can't create FMOD system" << std::endl;
        PrintFMODError(result);
        return result;
    }
    result = FMOD_Studio_System_Initialize(mFmodSystem, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
        std::cout << "Can't init FMOD system" << std::endl;
        PrintFMODError(result);
        return result;
    }
    return FMOD_OK;
}

FMOD_RESULT FMODWrapper::Update() const
{
    FMOD_RESULT result = FMOD_Studio_System_Update(mFmodSystem);
    if (result != FMOD_OK)
    {
        std::cout << "Couldn't update FMOD system" << std::endl;
        PrintFMODError(result);
    }
    return result;
}

FMODWrapper::~FMODWrapper()
{
    FMOD_Studio_System_Release(mFmodSystem);
    for (auto bankPair : mBankPairs)
    {
        FMOD_Studio_Bank_Unload(bankPair.bank);
        FMOD_Studio_Bank_Unload(bankPair.stringsBank);
    }
}

void FMODWrapper::PrintFMODError(FMOD_RESULT result) const
{
    std::cout << FMOD_ErrorString(result) << std::endl;
}

FMOD_RESULT FMODWrapper::LoadBank(const std::string &bankName)
{
    FMOD_RESULT result;
    FMOD_Bank_Pair bankPair;
    result = FMOD_Studio_System_LoadBankFile(mFmodSystem, std::format("Data/Audiobanks/{}.bank", bankName).c_str(), 0, &bankPair.bank);
    if (result != FMOD_OK)
    {
        std::cout << "Can't load bank" << std::endl;
        PrintFMODError(result);
        return result;
    }
    result = FMOD_Studio_System_LoadBankFile(mFmodSystem, std::format("Data/Audiobanks/{}.strings.bank", bankName).c_str(), 0, &bankPair.stringsBank);
    if (result != FMOD_OK)
    {
        std::cout << "Can't load strings bank" << std::endl;
        FMOD_Studio_Bank_Unload(bankPair.bank);
        PrintFMODError(result);
        return result;
    }
    mBankPairs.push_back(bankPair);
    return FMOD_OK;
}

FMOD_RESULT FMODWrapper::PlayBGM(const std::string &eventName)
{
    if (mBgmEventInstance != NULL)
    {
        StopBGM();
    }
    FMOD_RESULT result;
    result = FMOD_Studio_System_GetEvent(mFmodSystem, std::format("event:/{}", eventName).c_str(), &mBgmEventDescription);
    if (result != FMOD_OK)
    {
        std::cout << "Can't get event" << std::endl;
        PrintFMODError(result);
        return result;
    }
    result = FMOD_Studio_EventDescription_CreateInstance(mBgmEventDescription, &mBgmEventInstance);
    if (result != FMOD_OK)
    {
        std::cout << "Can't create an instance of the event" << std::endl;
        PrintFMODError(result);
        return result;
    }
    result = FMOD_Studio_EventInstance_Start(mBgmEventInstance);
    if (result != FMOD_OK)
    {
        std::cout << "Can't start the instance" << std::endl;
        PrintFMODError(result);
        return result;
    }

    return FMOD_OK;
}

FMOD_RESULT FMODWrapper::PauseBGM()
{
    if (mBgmEventInstance == NULL)
        return FMOD_OK;
    FMOD_RESULT result = FMOD_Studio_EventInstance_SetPaused(mBgmEventInstance, true);
    if (result != FMOD_OK)
    {
        std::cout << "Couldn't pause the bgm!" << std::endl;
        PrintFMODError(result);
    }
    return result;
}

FMOD_RESULT FMODWrapper::ResumeBGM()
{
    if (mBgmEventInstance == NULL)
        return FMOD_OK;
    FMOD_RESULT result = FMOD_Studio_EventInstance_SetPaused(mBgmEventInstance, false);
    if (result != FMOD_OK)
    {
        std::cout << "Couldn't resume the bgm!" << std::endl;
        PrintFMODError(result);
    }
    return result;
}

FMOD_RESULT FMODWrapper::StopBGM()
{
    if (mBgmEventInstance == NULL)
        return FMOD_OK;
    FMOD_RESULT result = FMOD_Studio_EventInstance_Stop(mBgmEventInstance, FMOD_STUDIO_STOP_IMMEDIATE);
    if (result != FMOD_OK)
    {
        std::cout << "Couldn't stop the bgm!" << std::endl;
        PrintFMODError(result);
    }
    FMOD_Studio_EventInstance_Release(mBgmEventInstance);
    mBgmEventInstance = NULL;
    return FMOD_OK;
}
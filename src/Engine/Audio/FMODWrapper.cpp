#include "FMODWrapper.h"

#include <iostream>
#include <format>
#include <fmod_studio.h>
#include <fmod.h>
#include <fmod_errors.h>

#include "../Logger.h"
#include "../Constants.h"

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
        LOG_ERROR("Can't create FMOD system");
        PrintFMODError(result);
        return result;
    }
    result = FMOD_Studio_System_Initialize(mFmodSystem, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
        LOG_ERROR("Can't init FMOD system");
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
        LOG_ERROR("Couldn't update FMOD system");
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
    LOG_WARNING(FMOD_ErrorString(result));
}

FMOD_RESULT FMODWrapper::LoadBank(const std::string &bankName)
{
    FMOD_RESULT result;
    FMOD_Bank_Pair bankPair;
    result = FMOD_Studio_System_LoadBankFile(mFmodSystem, std::format("{}/{}.bank", CONST_DATA_FOLDER, bankName).c_str(), 0, &bankPair.bank);
    if (result != FMOD_OK)
    {
        LOG_ERROR("Can't load bank");
        PrintFMODError(result);
        return result;
    }
    result = FMOD_Studio_System_LoadBankFile(mFmodSystem, std::format("{}/{}.strings.bank", CONST_DATA_FOLDER, bankName).c_str(), 0, &bankPair.stringsBank);
    if (result != FMOD_OK)
    {
        LOG_ERROR("Can't load strings bank");
        FMOD_Studio_Bank_Unload(bankPair.bank);
        PrintFMODError(result);
        return result;
    }
    mBankPairs.push_back(bankPair);
    return FMOD_OK;
}

FMOD_RESULT F_CALL FMODWrapper::EventCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters)
{
    void *userData = nullptr;
    FMOD_Studio_EventInstance_GetUserData(event, &userData);
    FMODWrapper *self = reinterpret_cast<FMODWrapper *>(userData);

    if (self)
    {
        self->OnEventCallback(type, event, parameters);
    }

    return FMOD_OK;
}

void FMODWrapper::OnEventCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters)
{
    if (type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER)
    {
        FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES *marker = (FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES *)parameters;
        std::cout << "Marker with name " << marker->name << " triggered at millisecond " << marker->position << std::endl;
    }
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
        LOG_WARNING("Can't get event");
        PrintFMODError(result);
        return result;
    }
    result = FMOD_Studio_EventDescription_CreateInstance(mBgmEventDescription, &mBgmEventInstance);
    if (result != FMOD_OK)
    {
        LOG_ERROR("Can't create an instance of the event");
        PrintFMODError(result);
        return result;
    }
    result = FMOD_Studio_EventInstance_Start(mBgmEventInstance);
    if (result != FMOD_OK)
    {
        LOG_ERROR("Can't start the instance");
        PrintFMODError(result);
        return result;
    }

    result = FMOD_Studio_EventInstance_SetUserData(mBgmEventInstance, this);
    if (result != FMOD_OK)
    {
        LOG_ERROR("Can't set user data on the instance");
        PrintFMODError(result);
        return result;
    }

    result = FMOD_Studio_EventInstance_SetCallback(mBgmEventInstance, EventCallback, FMOD_STUDIO_EVENT_CALLBACK_ALL);
    if (result != FMOD_OK)
    {
        LOG_ERROR("Can't set callback");
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
        LOG_ERROR("Couldn't pause the bgm!");
        PrintFMODError(result);
    }
    return result;
}

FMOD_RESULT FMODWrapper::ResumeBGM()
{
    if (mBgmEventInstance == NULL)
    {
        LOG_WARNING("Trying to resume an event instance that is not initialized.");
        return FMOD_OK;
    }
    FMOD_RESULT result = FMOD_Studio_EventInstance_SetPaused(mBgmEventInstance, false);
    if (result != FMOD_OK)
    {
        LOG_ERROR("Couldn't resume the bgm!");
        PrintFMODError(result);
    }
    return result;
}

FMOD_RESULT FMODWrapper::StopBGM()
{
    if (mBgmEventInstance == NULL)
    {
        LOG_WARNING("Trying to stop an event instance that is not initialized.");
        return FMOD_OK;
    }
    FMOD_RESULT result = FMOD_Studio_EventInstance_Stop(mBgmEventInstance, FMOD_STUDIO_STOP_IMMEDIATE);
    if (result != FMOD_OK)
    {
        LOG_ERROR("Couldn't stop the bgm!");
        PrintFMODError(result);
    }
    FMOD_Studio_EventInstance_Release(mBgmEventInstance);
    mBgmEventInstance = NULL;
    return FMOD_OK;
}
#ifdef USE_FMOD
#include "FMODWrapper.h"

#include <iostream>
#include <format>
#include <fmod_studio.h>
#include <fmod.h>
#include <fmod_errors.h>

#include "../Utils/Logger.h"
#include "../Packer/PackCatalog.h"
#include "../Exception.hpp"

#include "../Scripting/ScriptingManager.h"

namespace Mochi::Audio
{
    FMODWrapper::FMODWrapper(FS::PackCatalog *catalog, Scripting::ScriptingManager *scriptingManager) : mCatalog(catalog), mScripting(scriptingManager)
    {
        mFmodSystem = NULL;
        mBgmEventDescription = NULL;
        mBgmEventInstance = NULL;

        FMOD_RESULT result;

        result = FMOD_Studio_System_Create(&mFmodSystem, FMOD_VERSION);
        if (result != FMOD_OK)
        {
            throw SystemInitializationError("Audio", FMOD_ErrorString(result));
        }
        result = FMOD_Studio_System_Initialize(mFmodSystem, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
        if (result != FMOD_OK)
        {
            throw SystemInitializationError("Audio", FMOD_ErrorString(result));
        }
    }

    void FMODWrapper::Update(const float &dt)
    {
        FMOD_RESULT result = FMOD_Studio_System_Update(mFmodSystem);
        if (result != FMOD_OK)
        {
            LOG_ERROR(std::format("Couldn't update FMOD system: {}", FMOD_ErrorString(result)));
        }

        for (size_t i = 0; i < mEventQueue.size(); ++i)
        {
            std::cout << "Marker with name " << mEventQueue[i].MarkerName << " triggered at millisecond " << mEventQueue[i].MarkerPosition << std::endl;

            std::string code = std::format("GetEvent(\"AudioMarkerEvent\"):fire(\"{}\", {})", mEventQueue[i].MarkerName, mEventQueue[i].MarkerPosition);
            mScripting->Execute(code);
        }
        mEventQueue.clear();
    }

    FMODWrapper::~FMODWrapper()
    {
        for (auto bankPair : mBankPairs)
        {
            FMOD_Studio_Bank_Unload(bankPair.bank);
            FMOD_Studio_Bank_Unload(bankPair.stringsBank);
        }
        FMOD_Studio_System_Release(mFmodSystem);
    }

    void FMODWrapper::LoadBank(const std::string &bankName)
    {
        FMOD_RESULT result;
        FMOD_Bank_Pair bankPair;

        auto bankBuffer = mCatalog->GetFile(std::format("{}.bank", bankName));
        result = FMOD_Studio_System_LoadBankMemory(mFmodSystem, bankBuffer.data(), (int)bankBuffer.size(), FMOD_STUDIO_LOAD_MEMORY_MODE::FMOD_STUDIO_LOAD_MEMORY, 0, &bankPair.bank);
        if (result != FMOD_OK)
        {
            throw ResourceNotFoundError("Audio bank", FMOD_ErrorString(result));
        }
        auto stringsBankBuffer = mCatalog->GetFile(std::format("{}.strings.bank", bankName));
        result = FMOD_Studio_System_LoadBankMemory(mFmodSystem, stringsBankBuffer.data(), (int)stringsBankBuffer.size(), FMOD_STUDIO_LOAD_MEMORY_MODE::FMOD_STUDIO_LOAD_MEMORY, 0, &bankPair.stringsBank);
        if (result != FMOD_OK)
        {
            result = FMOD_Studio_Bank_Unload(bankPair.bank);
            throw ResourceNotFoundError("Audio string bank", FMOD_ErrorString(result));
        }
        mBankPairs.push_back(bankPair);
        mBankPairsMap[bankName] = bankPair;
    }

    void FMODWrapper::UnloadBank(const std::string &bankName)
    {
        if (mBankPairsMap.find(bankName) != mBankPairsMap.end())
        {
            auto pair = mBankPairsMap.at(bankName);
            mBankPairs.erase(
                std::remove_if(mBankPairs.begin(), mBankPairs.end(),
                               [&](const FMOD_Bank_Pair &currentPair)
                               { return currentPair.bank == pair.bank; }),
                mBankPairs.end());
            ClearBusCache();
            FMOD_RESULT result = FMOD_Studio_Bank_Unload(pair.bank);
            if (result != FMOD_OK)
            {
                throw AudioOperationError("Unload bank", FMOD_ErrorString(result));
            }
            result = FMOD_Studio_Bank_Unload(pair.stringsBank);
            if (result != FMOD_OK)
            {
                throw AudioOperationError("Unload bank", FMOD_ErrorString(result));
            }
        }
        else
        {
            LOG_WARNING(std::format("The bank with name {} cannot be unloaded because it is not loaded", bankName));
        }
    }

    FMOD_RESULT F_CALL FMODWrapper::EventCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters)
    {
        void *userData = nullptr;
        FMOD_Studio_EventInstance_GetUserData(event, &userData);
        FMODWrapper *self = static_cast<FMODWrapper *>(userData);

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
            mEventQueue.push_back({marker->name, marker->position});
        }
    }

    void FMODWrapper::PlayBGM(const std::string &eventName)
    {
        if (mBgmEventInstance != NULL)
        {
            StopBGM();
        }
        FMOD_RESULT result;
        result = FMOD_Studio_System_GetEvent(mFmodSystem, std::format("event:/{}", eventName).c_str(), &mBgmEventDescription);
        if (result != FMOD_OK)
        {
            throw AudioOperationError("Get event", FMOD_ErrorString(result));
        }
        result = FMOD_Studio_EventDescription_CreateInstance(mBgmEventDescription, &mBgmEventInstance);
        if (result != FMOD_OK)
        {
            throw AudioOperationError("Event create instance", FMOD_ErrorString(result));
        }
        result = FMOD_Studio_EventInstance_Start(mBgmEventInstance);
        if (result != FMOD_OK)
        {
            throw AudioOperationError("Event instance start", FMOD_ErrorString(result));
        }

        result = FMOD_Studio_EventInstance_SetUserData(mBgmEventInstance, this);
        if (result != FMOD_OK)
        {
            throw AudioOperationError("Event instance set user data", FMOD_ErrorString(result));
        }

        result = FMOD_Studio_EventInstance_SetCallback(mBgmEventInstance, EventCallback, FMOD_STUDIO_EVENT_CALLBACK_ALL);
        if (result != FMOD_OK)
        {
            throw AudioOperationError("Event instance set callback", FMOD_ErrorString(result));
        }
    }

    void FMODWrapper::PauseBGM()
    {
        if (mBgmEventInstance == NULL)
        {
            LOG_WARNING("Trying to resume an event instance that is not initialized.");
            return;
        }
        FMOD_RESULT result = FMOD_Studio_EventInstance_SetPaused(mBgmEventInstance, true);
        if (result != FMOD_OK)
        {
            throw AudioOperationError("Pause BGM", FMOD_ErrorString(result));
        }
    }

    void FMODWrapper::ResumeBGM()
    {
        if (mBgmEventInstance == NULL)
        {
            LOG_WARNING("Trying to resume an event instance that is not initialized.");
            return;
        }
        FMOD_RESULT result = FMOD_Studio_EventInstance_SetPaused(mBgmEventInstance, false);
        if (result != FMOD_OK)
        {
            throw AudioOperationError("Resume BGM", FMOD_ErrorString(result));
        }
    }

    void FMODWrapper::StopBGM()
    {
        if (mBgmEventInstance == NULL)
        {
            LOG_WARNING("Trying to stop an event instance that is not initialized.");
            return;
        }
        FMOD_RESULT result = FMOD_Studio_EventInstance_Stop(mBgmEventInstance, FMOD_STUDIO_STOP_ALLOWFADEOUT);
        if (result != FMOD_OK)
        {
            throw AudioOperationError("Stop BGM", FMOD_ErrorString(result));
        }
        FMOD_Studio_EventInstance_Release(mBgmEventInstance);
        mBgmEventInstance = NULL;
    }

    void FMODWrapper::SkipToTimelinePosition(const int &ms)
    {
        if (mBgmEventInstance == NULL)
        {
            LOG_WARNING("Trying to skip on the timeline of an event instance that is not initialized.");
            return;
        }
        FMOD_RESULT result = FMOD_Studio_EventInstance_SetTimelinePosition(mBgmEventInstance, ms);
        if (result != FMOD_OK)
        {
            throw AudioOperationError("Skip timeline position", FMOD_ErrorString(result));
        }
    }

    void FMODWrapper::PlayOneShot(const std::string &eventName)
    {
        FMOD_RESULT result;
        FMOD_STUDIO_EVENTDESCRIPTION *eventDescription = NULL;

        if (mSoundCache.find(eventName) != mSoundCache.end())
        {
            eventDescription = mSoundCache.at(eventName);
        }
        else
        {
            result = FMOD_Studio_System_GetEvent(mFmodSystem, std::format("event:/{}", eventName).c_str(), &eventDescription);
            if (result != FMOD_OK)
            {
                throw AudioOperationError("Get event", FMOD_ErrorString(result));
            }
            mSoundCache[eventName] = eventDescription;
        }

        FMOD_STUDIO_EVENTINSTANCE *eventInstance = NULL;
        result = FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance);
        if (result != FMOD_OK)
        {
            throw AudioOperationError("Event create instance", FMOD_ErrorString(result));
        }
        result = FMOD_Studio_EventInstance_Start(eventInstance);
        if (result != FMOD_OK)
        {
            throw AudioOperationError("Event instance start", FMOD_ErrorString(result));
        }

        result = FMOD_Studio_EventInstance_Release(eventInstance);
        if (result != FMOD_OK)
        {
            throw AudioOperationError("Event instance release", FMOD_ErrorString(result));
        }
    }

    void FMODWrapper::SetParameter(const std::string &parameterName, const float &value)
    {
        FMOD_RESULT result = FMOD_Studio_System_SetParameterByName(mFmodSystem, parameterName.c_str(), value, false);
        if (result != FMOD_OK)
        {
            throw AudioOperationError("Set parameter", FMOD_ErrorString(result));
        }
    }

    void FMODWrapper::SetBusVolume(const std::string &busGroupName, const float &value)
    {
        FMOD_STUDIO_BUS *bus;
        FMOD_RESULT result;
        if (mBusesCache.find(busGroupName) != mBusesCache.end())
        {
            bus = mBusesCache.at(busGroupName);
        }
        else
        {
            result = FMOD_Studio_System_GetBus(mFmodSystem, std::format("bus:/{}", busGroupName).c_str(), &bus);
            if (result != FMOD_OK)
            {
                throw AudioOperationError("Get bus", FMOD_ErrorString(result));
            }
            mBusesCache[busGroupName] = bus;
        }

        result = FMOD_Studio_Bus_SetVolume(bus, value);
        if (result != FMOD_OK)
        {
            throw AudioOperationError("Set bus volume", FMOD_ErrorString(result));
        }
    }
}
#endif
#include "World.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <filesystem>
#include <fmod_studio.h>
#include <fmod_studio_common.h>
#include <fmod.h>
#include <fmod_common.h>
#include <fmod_errors.h>

#include "Graphics/Sprite.h"

void PrintFMODError(FMOD_RESULT result)
{
    std::cout << FMOD_ErrorString(result) << std::endl;
}

World::World()
{
    _timesUpdated = 0;
    bWasDown = false;

    SDL_SetAppMetadata("Space Shooter", "0.1", "com.magicmochi.spaceshooter");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        throw SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Space Shooter", 640, 480, SDL_WINDOW_ALWAYS_ON_TOP, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        throw SDL_APP_FAILURE;
    }

    if (!SDL_SetRenderVSync(renderer, 1))
    {
        SDL_Log("Couldn't set vsyinc to true: %s", SDL_GetError());
        throw SDL_APP_FAILURE;
    }

    mSampleSprite = new Sprite(renderer, "Data\\Coproducers.jpg");

    FMOD_RESULT result;
    mFmodSystem = NULL;
    result = FMOD_Studio_System_Create(&mFmodSystem, FMOD_VERSION);
    if (result != FMOD_OK)
    {
        std::cout << "Can't create FMOD system" << std::endl;
        PrintFMODError(result);
        exit(-1);
    }
    result = FMOD_Studio_System_Initialize(mFmodSystem, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
        std::cout << "Can't init FMOD system" << std::endl;
        PrintFMODError(result);
        exit(-1);
    }
    result = FMOD_Studio_System_LoadBankFile(mFmodSystem, "Data/Audiobanks/Master.bank", 0, &mMainBank);
    if (result != FMOD_OK)
    {
        std::cout << "Can't load bank" << std::endl;
        PrintFMODError(result);
        exit(-1);
    }
    result = FMOD_Studio_System_LoadBankFile(mFmodSystem, "Data/Audiobanks/Master.strings.bank", 0, &mStringsBank);
    if (result != FMOD_OK)
    {
        std::cout << "Can't load strings bank" << std::endl;
        PrintFMODError(result);
        exit(-1);
    }
}

void World::Update(const float &dt)
{
    const bool *keyboardState = SDL_GetKeyboardState(NULL);

    if (keyboardState[SDL_SCANCODE_A] == true)
    {
        std::cout << "A is pressed!";
    }

    _timesUpdated++;
    // std::cout << "Times updated is: " << _timesUpdated << std::endl;

    FMOD_RESULT result = FMOD_Studio_System_Update(mFmodSystem);
    if (result != FMOD_OK)
    {
        std::cout << "Couldn't update FMOD system" << std::endl;
        PrintFMODError(result);
    }

    if (keyboardState[SDL_SCANCODE_B] == true)
    {
        if (!bWasDown)
        {
            std::cout << "B is pressed!" << std::endl;
            FMOD_STUDIO_EVENTDESCRIPTION *eventDescription = NULL;
            FMOD_STUDIO_EVENTINSTANCE *eventInstance = NULL;

            FMOD_STUDIO_EVENTDESCRIPTION *eventDescriptionList = NULL;
            int count = 0;
            result = FMOD_Studio_Bank_GetEventList(mMainBank, &eventDescriptionList, 5, &count);
            if (result != FMOD_OK)
            {
                std::cout << "Can't get event list" << std::endl;
                PrintFMODError(result);
                bWasDown = false;
                return;
            }

            result = FMOD_Studio_System_GetEvent(mFmodSystem, "event:/TestMusic", &eventDescription);
            if (result != FMOD_OK)
            {
                std::cout << "Can't get event" << std::endl;
                PrintFMODError(result);
                bWasDown = false;
                return;
            }
            result = FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance);
            if (result != FMOD_OK)
            {
                std::cout << "Can't create an instance of the event" << std::endl;
                PrintFMODError(result);
                bWasDown = false;
                return;
            }
            result = FMOD_Studio_EventInstance_Start(eventInstance);
            if (result != FMOD_OK)
            {
                std::cout << "Can't start the instance" << std::endl;
                PrintFMODError(result);
                bWasDown = false;
                return;
            }
            FMOD_Studio_EventInstance_Release(eventInstance);
        }
        bWasDown = true;
    }
    else
    {
        bWasDown = false;
    }
}

void World::AppEvent(SDL_Event *event)
{
}

void World::Render() const
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    ///////////////////////////////

    mSampleSprite->Render(renderer);

    // Dev build message
    int w = 0;
    int h = 0;

#ifdef DEBUG
    SDL_GetRenderOutputSize(renderer, &w, &h);
    SDL_SetRenderScale(renderer, 1, 1);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    if (!SDL_RenderDebugText(renderer, 5, h - 10, "DEVELOPMENT BUILD"))
    {
        std::cout << "Can't render text? Error is: " << SDL_GetError() << std::endl;
    }
#endif
    ///////////////////////////////
    SDL_RenderPresent(renderer); /* put it all on the screen! */
}

World::~World()
{
    delete mSampleSprite;
    FMOD_Studio_System_Release(mFmodSystem);
    FMOD_Studio_Bank_Unload(mMainBank);
}
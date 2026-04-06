#include "SDLSystemEventDispatcher.h"

#include "SDL3/SDL_events.h"

#include "EventBus.h"
#include "EngineEvents.h"

namespace Mochi::Event
{
    SDLSystemEventDispatcher::SDLSystemEventDispatcher(EventBus *eventBus) : mEventBus(eventBus)
    {
    }

    SDLSystemEventDispatcher::~SDLSystemEventDispatcher()
    {
    }

    void SDLSystemEventDispatcher::PollEvents() const
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                mEventBus->Publish<ApplicationQuitEvent>({});
            }
            else if (event.type == SDL_EVENT_GAMEPAD_ADDED)
            {
                mEventBus->Publish<GamepadAddedEvent>({event.gdevice.which});
            }
            else if (event.type == SDL_EVENT_GAMEPAD_REMOVED)
            {
                mEventBus->Publish<GamepadRemovedEvent>({event.gdevice.which});
            }

            mEventBus->Publish<SDL_Event>(event);
        }
    }

}
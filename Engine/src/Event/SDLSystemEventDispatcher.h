#ifndef HDEF_SDLSYSTEMEVENTDISPATCHER
#define HDEF_SDLSYSTEMEVENTDISPATCHER

#include "ISystemEventDispatcher.h"

namespace Mochi::Event
{
    class EventBus;
    /**
     * @brief Implementation of the ISystemEventDispatcher using the SDL_PollEvent method to retrieve the operative system events.
     */
    class SDLSystemEventDispatcher : public ISystemEventDispatcher
    {
    private:
        EventBus *mEventBus;

    public:
        /// @param eventBus The main engine default event bus.
        SDLSystemEventDispatcher(EventBus *eventBus);
        virtual ~SDLSystemEventDispatcher();
        /// @brief Polls the OS events and dispatches events using the EventBus.
        ///
        /// Events dispatched:
        ///
        /// - ApplicationQuitEvent
        ///
        /// - GamepadAddedEvent
        ///
        /// - GamepadRemovedEvent
        virtual void PollEvents() const;
    };
}

#endif
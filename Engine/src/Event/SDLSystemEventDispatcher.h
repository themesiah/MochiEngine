#ifndef HDEF_SDLSYSTEMEVENTDISPATCHER
#define HDEF_SDLSYSTEMEVENTDISPATCHER

#include "ISystemEventDispatcher.h"

namespace Mochi::Event
{
    class EventBus;
    class SDLSystemEventDispatcher : public ISystemEventDispatcher
    {
    private:
        EventBus *mEventBus;

    public:
        SDLSystemEventDispatcher(EventBus *eventBus);
        virtual ~SDLSystemEventDispatcher();
        virtual void PollEvents() const;
    };
}

#endif
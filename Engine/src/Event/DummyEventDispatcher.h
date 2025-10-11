#ifndef HDEF_DUMMYEVENTDISPATCHER
#define HDEF_DUMMYEVENTDISPATCHER

#include "ISystemEventDispatcher.h"

namespace Mochi::Event
{
    class DummyEventDispatcher : public ISystemEventDispatcher
    {
    public:
        virtual ~DummyEventDispatcher() {}
        virtual void PollEvents() const override {};
    };
}

#endif
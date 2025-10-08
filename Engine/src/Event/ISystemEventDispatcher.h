#ifndef ISYSTEMEVENTDISPATCHER
#define ISYSTEMEVENTDISPATCHER

namespace Mochi::Event
{
    class ISystemEventDispatcher
    {
    public:
        virtual ~ISystemEventDispatcher() {}
        virtual void PollEvents() const = 0;
    };
}

#endif
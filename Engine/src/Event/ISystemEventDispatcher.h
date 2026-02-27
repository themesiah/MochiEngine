#ifndef ISYSTEMEVENTDISPATCHER
#define ISYSTEMEVENTDISPATCHER

namespace Mochi::Event
{
    /**
     * @brief Interface to poll the events of the operative system (input, window controls, application exit...)
     */
    class ISystemEventDispatcher
    {
    public:
        virtual ~ISystemEventDispatcher() {}
        /// @brief Implement this with the polling of events of the operative system. This will be called once per frame in the Engine class.
        virtual void PollEvents() const = 0;
    };
}

#endif
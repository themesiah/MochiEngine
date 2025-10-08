#ifndef HDEF_EVENTBUS
#define HDEF_EVENTBUS

#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <unordered_map>
#include <typeindex>

namespace Mochi::Event
{
    using SubscriptionHandler = size_t;
    using SubscriberFunc = std::function<void(const void *)>;
    using SubscriberPair = std::pair<SubscriptionHandler, SubscriberFunc>;
    class EventBus
    {
    private:
        SubscriptionHandler mLastSubscriptionHandler;
        std::vector<SubscriptionHandler> mFreeSubscriptionHandlers;
        std::unordered_map<std::type_index, std::vector<SubscriberPair>> mSubscribersMap;

        SubscriptionHandler GetNextSubscriptionHandler()
        {
            if (mFreeSubscriptionHandlers.size() == 0)
            {
                return mLastSubscriptionHandler++;
            }
            else
            {
                SubscriptionHandler free = mFreeSubscriptionHandlers.back();
                mFreeSubscriptionHandlers.pop_back();
                return free;
            }
        }

    public:
        EventBus() : mLastSubscriptionHandler(1) {}

        template <typename EventType>
        SubscriptionHandler Subscribe(std::function<void(const EventType &)> callback)
        {
            auto &subscribers = mSubscribersMap[typeid(EventType)];
            auto handler = GetNextSubscriptionHandler();
            subscribers.emplace_back(handler, [callback](const void *e)
                                     { callback(*static_cast<const EventType *>(e)); });
            return handler;
        }
        template <typename EventType>
        void Unsubscribe(SubscriptionHandler handler)
        {
            auto it = mSubscribersMap.find(typeid(EventType));
            if (it == mSubscribersMap.end())
                return; // There was not an event of that type

            auto &subscribers = it->second;
            auto was = subscribers.size();
            subscribers.erase(
                std::remove_if(subscribers.begin(), subscribers.end(),
                               [handler](auto &pair)
                               { return pair.first == handler; }),
                subscribers.end());
            if (was != subscribers.size())
                mFreeSubscriptionHandlers.push_back(handler);
        }
        template <typename EventType>
        void Publish(const EventType &event)
        {
            auto it = mSubscribersMap.find(typeid(EventType));
            if (it != mSubscribersMap.end())
            {
                for (auto &[id, subscriber] : it->second)
                {
                    subscriber(&event);
                }
            }
        }
    };
}

#endif
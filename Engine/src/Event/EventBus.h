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
    /**
     * @brief The event bus is a Pub-Sub implementation that accepts any type as event data.
     *
     * A subscription handler is the identifier of the subscription, needed on a per-subscription basis in order to unsubscribe for it.
     *
     * Guarantees:
     *
     * - Anyone can subscribe to any type of event at any time.
     *
     * - Anyone can publish any type of event at any time.
     *
     * - Anyone can unsubscribe from an event at any time.
     *
     * Non responsible:
     *
     * - Publishing to an event where a destroyed class was subscribed and didn't unsubscribe can cause unexpected consecuences.
     */
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
        ~EventBus()
        {
            mFreeSubscriptionHandlers.clear();
            mSubscribersMap.clear();
        }

        /**
         * @brief Subscribe to a specific event type.
         * @param EventType The event type to which to subscribe. Whatever type works. It is recommended to use a type (struct or class) specifically created for that event.
         * @param callback The callback to be called when the event is published.
         */
        template <typename EventType>
        SubscriptionHandler Subscribe(std::function<void(const EventType &)> callback)
        {
            auto &subscribers = mSubscribersMap[typeid(EventType)];
            auto handler = GetNextSubscriptionHandler();
            subscribers.emplace_back(handler, [callback](const void *e)
                                     { callback(*static_cast<const EventType *>(e)); });
            return handler;
        }

        /**
         * @brief Unsubscribe from a specific event. If the event type didn't exist or the handler wasn't subscribed, nothing happenns.
         * @param EventType The event type from which to unsubscribe.
         * @param handler The identifier of the subscription.
         */
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

        /**
         * @brief Publish an event, triggering all callbacks subscribed to it with the provided data.
         * @param EventType The event type to which to publish.
         * @param event The event data of type EventType.
         */
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
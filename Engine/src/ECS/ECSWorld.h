#ifndef HDEF_ECSWORLD
#define HDEF_ECSWORLD

#include <entt/entt.hpp>

#include <vector>

#include "IECSSystem.h"

namespace Mochi::ECS
{
    struct TestComponent
    {
        int Data;
        int OtherData;
    };

    using EntityType = entt::registry::entity_type;
    class ECSWorld
    {
    private:
        entt::registry mRegistry;
        entt::dispatcher mDispatcher;
        std::vector<SystemPtr> mSystems;

    public:
        ECSWorld();
        virtual ~ECSWorld();

        template <typename TSystem, typename... Args>
        void RegisterSystem(Args &&...args)
        {
            static_assert(std::is_base_of_v<IECSSystem, TSystem>,
                          "TSystem must inherit from IECSSystem");
            mSystems.push_back(std::make_unique<TSystem>(
                mRegistry,
                mDispatcher,
                std::forward<Args>(args)...));

            // Sort the system list each time a system is registered.
            // This is to avoid sorting them each update.
            // We won't be registering / unregistering systems every time, so this is not performance critical.
            std::sort(mSystems.begin(), mSystems.end(),
                      [](SystemPtr &a, SystemPtr &b)
                      {
                          return a->GetPriority() > b->GetPriority();
                      });
        }
        EntityType CreateEntity();

        template <typename ComponentType, typename... Args>
        void Set(EntityType entity, Args &&...args)
        {
            mRegistry.emplace<ComponentType>(entity, std::forward<Args>(args)...);
        }

        void Update(const float &dt);
    };
}

#endif
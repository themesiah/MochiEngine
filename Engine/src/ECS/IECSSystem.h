#ifndef HDEF_IECSSYSTEM
#define HDEF_IECSSYSTEM

#include <memory>
#include <entt/entt.hpp>

namespace Mochi::ECS
{
    class IECSSystem
    {
    public:
        IECSSystem(entt::registry &registry, entt::dispatcher &dispatcher) : mRegistry(registry), mDispatcher(dispatcher) {}
        ~IECSSystem() {}
        virtual void Update(const float &dt) = 0;
        virtual int GetPriority() const = 0;

    protected:
        entt::registry &mRegistry;
        entt::dispatcher &mDispatcher;
    };
    using SystemPtr = std::unique_ptr<IECSSystem>;
}

#endif
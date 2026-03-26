#include "ECSCharacterControllerSystem.h"

#include "../ECSWorld.h"
#include "../Components/CharacterController.hpp"

namespace Mochi::ECS
{
    CharacterControllerSystem::CharacterControllerSystem(entt::registry &registry, entt::dispatcher &dispatcher, ECSWorld &world)
        : IECSSystem(registry, dispatcher), mWorld(world)
    {
    }

    CharacterControllerSystem::~CharacterControllerSystem()
    {
    }

    void CharacterControllerSystem::Update(const float &dt)
    {
        auto view = mRegistry.view<CharacterController>();

        view.each([&](const entt::entity &entity, CharacterController &cc)
                  { cc.Update(dt, entity, mRegistry); });
    }

    int CharacterControllerSystem::GetPriority() const
    {
        return 3;
    }
}
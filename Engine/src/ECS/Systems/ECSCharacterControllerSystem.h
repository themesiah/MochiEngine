#ifndef HDEF_ECSCHARACTERCONTROLLERSYSTEM
#define HDEF_ECSCHARACTERCONTROLLERSYSTEM

#include "../IECSSystem.h"

namespace Mochi::ECS
{
    class ECSWorld;
    class CharacterControllerSystem : public IECSSystem
    {
    private:
        ECSWorld &mWorld;

        void OnCharacterControllerConstructed(entt::registry &, entt::entity);

    public:
        CharacterControllerSystem(entt::registry &registry, entt::dispatcher &dispatcher, ECSWorld &world);
        virtual ~CharacterControllerSystem();
        virtual void Update(const float &dt) override;
        virtual int GetPriority() const override;
    };
}

#endif
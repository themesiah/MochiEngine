#include "ECSWorld.h"

#include <algorithm>

#include "../Utils/Logger.h"
#include "Systems/ECSRenderSystem.h"
#include "Systems/ECSAnimationSystem.h"

namespace Mochi::ECS
{
    ECSWorld::ECSWorld() : mRegistry(), mDispatcher(), mSystems()
    {
        RegisterSystem<RenderSystem>();
        RegisterSystem<AnimationSystem>();
    }

    ECSWorld::~ECSWorld()
    {
    }

    EntityType ECSWorld::CreateEntity()
    {
        return mRegistry.create();
    }

    void ECSWorld::Update(const float &dt)
    {
        for (auto &s : mSystems)
        {
            s->Update(dt);
        }
    }
}
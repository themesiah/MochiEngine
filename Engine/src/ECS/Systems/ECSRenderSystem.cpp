#include "ECSRenderSystem.h"

#include "../../Engine.h"
#include "../Components/ECSTransform.h"
#include "../Components/ECSSprite.h"
#include "../../Graphics/RenderCommand.h"

namespace Mochi::ECS
{

    RenderSystem::RenderSystem(entt::registry &registry, entt::dispatcher &dispatcher) : IECSSystem(registry, dispatcher)
    {
    }

    RenderSystem::~RenderSystem()
    {
    }

    void RenderSystem::Update(const float &dt)
    {
        auto view = mRegistry.view<const TransformComponent, SpriteComponent>();

        Engine &e = Engine::Get();
        view.each(
            [&e](const TransformComponent &t, SpriteComponent &s)
            {
                Graphics::RenderCommand rc;
                rc.texture = s.Texture;
                rc.zindex = s.ZIndex;
                rc.sourceRect = s.SrcRect;
                rc.destRect.SetPosition(t.Position);
                rc.destRect.SetSize(rc.sourceRect.GetSize() * t.Scale);
                rc.flip = s.Flip;
                rc.angle = s.Angle;
                e.AddRenderCommand(rc);
            });
    }

    int RenderSystem::GetPriority() const
    {
        return 0;
    }
}
#ifndef HDEF_PLAT_GAMELAYER
#define HDEF_PLAT_GAMELAYER

#include "Layer.h"

#include <memory>
#include <vector>

#include "Graphics/AbstractTextureFactory.h"
#include "Graphics/IAnimationFactory.h"
#include "ECS/ECSWorld.h"

namespace Mochi::Platformer
{
    class GameLayer : public Layer
    {
    private:
        std::unique_ptr<Graphics::AbstractTextureFactory> mTextureFactory;
        std::unique_ptr<Graphics::IAnimationFactory> mAnimationFactory;
        ECS::EntityType mPlayerEntity;
        std::vector<ECS::EntityType> mBlocksEntities;

    public:
        GameLayer();
        virtual ~GameLayer();
        virtual void InitLayer() override;
        virtual bool Update(const float &dt) override;
        virtual void Render() const override;
        virtual void GUI() override;
    };
}

#endif
#ifndef HDEF_PLAT_GAMELAYER
#define HDEF_PLAT_GAMELAYER

#include "Layer.h"

#include <memory>
#include <vector>

#include "ECS/ECSWorld.h"

namespace Mochi::Graphics
{
    class AbstractTextureFactory;
    class IAnimationFactory;
}

namespace Mochi::Platformer
{
    class Tilemap;
    class GameLayer : public Layer
    {
    private:
        std::unique_ptr<Graphics::AbstractTextureFactory> mTextureFactory;
        std::unique_ptr<Graphics::IAnimationFactory> mAnimationFactory;
        ECS::EntityType mPlayerEntity;
        std::vector<ECS::EntityType> mBlocksEntities;
        std::vector<ECS::EntityType> mEnemyEntities;
        std::unique_ptr<Tilemap> mTilemap;

    public:
        GameLayer();
        virtual ~GameLayer();
        virtual void InitLayer() override;
        virtual bool Update(const float &dt) override;
        virtual void Render() const override;
        virtual void GUI() override;

#if DEBUG
        virtual void Debug() const override;
#endif
    };
}

#endif
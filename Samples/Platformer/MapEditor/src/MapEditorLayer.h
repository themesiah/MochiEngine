#ifndef HDEF_MAPEDITORLAYER
#define HDEF_MAPEDITORLAYER

#include <memory>

#include "Layer.h"

#include "Event/EventBus.h"
#include "Tilemap/Tilemap.h"

namespace Mochi::Graphics
{
    class AbstractTextureFactory;
    class IAnimationFactory;
}
namespace Mochi::Platformer::Editor
{
    class MapEditor;
    class MapEditorLayer : public Layer
    {
    private:
        Event::SubscriptionHandler mEventPollingSubscription;
        std::unique_ptr<Tilemap> mTilemap;
        std::unique_ptr<Graphics::AbstractTextureFactory> mTextureFactory;
        std::unique_ptr<Graphics::IAnimationFactory> mAnimationFactory;
        std::unique_ptr<MapEditor> mMapEditor;

    public:
        MapEditorLayer();
        virtual ~MapEditorLayer();
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
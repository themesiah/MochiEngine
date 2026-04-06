#ifndef HDEF_MAPEDITORLAYER
#define HDEF_MAPEDITORLAYER

#include "Layer.h"

#include "Event/EventBus.h"

namespace Mochi::Platformer::Editor
{
    class MapEditorLayer : public Layer
    {
    private:
        Event::SubscriptionHandler mEventPollingSubscription;

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
#ifndef HDEF_PLAT_GAMELAYER
#define HDEF_PLAT_GAMELAYER

#include "Layer.h"

namespace Mochi::Platformer
{
    class GameLayer : public Layer
    {
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
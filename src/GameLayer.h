#ifndef HDEF_GAMELAYER
#define HDEF_GAMELAYER

#include "Layer.h"

namespace Mochi::Shooter
{
    class GameLayer : public Layer
    {
        GameLayer();
        ~GameLayer();
        virtual bool Update(const float &dt) override;
        virtual void Render() const override;
    };
}

#endif
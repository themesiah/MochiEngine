#include "GameLayer.h"
#include "Layer.h"

namespace Mochi::Shooter
{
    GameLayer::GameLayer() : Layer()
    {
    }

    GameLayer::~GameLayer()
    {
    }

    bool GameLayer::Update(const float &dt)
    {
        return true;
    }

    void GameLayer::Render() const
    {
    }

}
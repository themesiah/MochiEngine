
#ifndef HDEF_SPACESHOOTERENGINE
#define HDEF_SPACESHOOTERENGINE

#include "Engine.h"

#include <memory>

namespace Mochi::Shooter
{
    class Player;
    class PointsSystem;
    class SpaceShooterEngine : public Engine
    {
    private:
        std::shared_ptr<Player> mPlayer;
        std::unique_ptr<PointsSystem> mPointsSystem;

    public:
        SpaceShooterEngine(const char *appName, const char *appVersion, const char *appId, const char *windowName);
        virtual ~SpaceShooterEngine();

    protected:
        virtual bool OnUpdate(const float &dt);
        virtual void OnRender() const;
    };
}

#endif
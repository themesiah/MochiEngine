
#ifndef HDEF_SPACESHOOTERENGINE
#define HDEF_SPACESHOOTERENGINE

#include "Engine.h"

#include <memory>

namespace Mochi::Shooter
{
    class Player;
    class PointsSystem;
    class Enemy;
    class SpaceShooterEngine : public Engine
    {
    private:
        std::shared_ptr<Player> mPlayer;
        std::unique_ptr<PointsSystem> mPointsSystem;
        std::shared_ptr<Enemy> mEnemy;

    public:
        SpaceShooterEngine(const char *appName, const char *appVersion, const char *appId, const char *windowName);
        virtual ~SpaceShooterEngine();

    protected:
        virtual bool OnUpdate(const float &dt);
        virtual void OnRender() const;
    };
}

#endif
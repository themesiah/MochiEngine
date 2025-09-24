
#ifndef HDEF_SPACESHOOTERENGINE
#define HDEF_SPACESHOOTERENGINE

#include "Engine.h"

#include <memory>

namespace Mochi::Shooter
{
    class Player;
    class SpaceShooterEngine : public Engine
    {
    private:
        // TEMP
        std::shared_ptr<Mochi::Graphics::Sprite> mSampleSprite;
        std::shared_ptr<Mochi::Graphics::AnimatedSprite> mAnimatedSprite;
        Mochi::EntityHandler mSpriteHandler;
        Mochi::EntityHandler mAnimatedSpriteHandler;
        // END TEMP
        std::shared_ptr<Player> mPlayer;
        Mochi::EntityHandler mPlayerHandler;

    public:
        SpaceShooterEngine(const char *appName, const char *appVersion, const char *appId, const char *windowName);
        virtual ~SpaceShooterEngine();

    protected:
        virtual bool OnUpdate(const float &dt);
    };
}

#endif
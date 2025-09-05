
#ifndef HDEF_SPACESHOOTERENGINE
#define HDEF_SPACESHOOTERENGINE

#include "Engine.h"

#include <memory>

class SpaceShooterEngine : public Mochi::Engine
{
private:
    // TEMP
    std::shared_ptr<Mochi::Graphics::Sprite> mSampleSprite;
    std::shared_ptr<Mochi::Graphics::AnimatedSprite> mAnimatedSprite;
    Mochi::EntityHandler mSpriteHandler;
    Mochi::EntityHandler mAnimatedSpriteHandler;
    // END TEMP
public:
    SpaceShooterEngine(const char *appName, const char *appVersion, const char *appId, const char *windowName);
    virtual ~SpaceShooterEngine();

protected:
    virtual bool OnUpdate(const float &dt);
};

#endif
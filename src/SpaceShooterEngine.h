
#ifndef HDEF_SPACESHOOTERENGINE
#define HDEF_SPACESHOOTERENGINE

#include "Engine.h"

#include <memory>

class SpaceShooterEngine : public Engine
{
private:
    // TEMP
    std::shared_ptr<Sprite> mSampleSprite;
    std::shared_ptr<AnimatedSprite> mAnimatedSprite;
    EntityHandler mSpriteHandler;
    EntityHandler mAnimatedSpriteHandler;
    // END TEMP
public:
    SpaceShooterEngine();
    virtual ~SpaceShooterEngine();

protected:
    virtual bool OnUpdate(const float &dt);
};

#endif
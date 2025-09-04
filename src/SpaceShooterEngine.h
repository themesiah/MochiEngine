
#ifndef HDEF_SPACESHOOTERENGINE
#define HDEF_SPACESHOOTERENGINE

#include "Engine.h"

class SpaceShooterEngine : public Engine
{
public:
    SpaceShooterEngine();
    virtual ~SpaceShooterEngine();

protected:
    virtual bool OnUpdate(const float &dt);
};

#endif
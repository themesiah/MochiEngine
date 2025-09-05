#include "SpaceShooterEngine.h"

int main()
{
    const char *appName = "Space Shooter";
    const char *appVersion = "0.1";
    const char *appId = "com.magicmochi.spaceshooter";
    const char *windowName = "Space Shooter";
    SpaceShooterEngine engine(appName, appVersion, appId, windowName);

    while (engine.Update())
    {
    }
}
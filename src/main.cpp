#include "SpaceShooterEngine.h"
#ifdef _MSC_VER
#include <Windows.h>
#endif

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

#ifdef _MSC_VER
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return main();
}
#endif
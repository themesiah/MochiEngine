#ifdef _MSC_VER
#include <Windows.h>
#endif

#include "Engine.h"
#include "GameLayer.h"

int main()
{
    const char *appName = "Space Shooter";
    const char *appVersion = "0.1";
    const char *appId = "com.magicmochi.spaceshooter";
    const char *windowName = "Space Shooter";
    Mochi::Engine engine(appName, appVersion, appId, windowName);
    Mochi::Shooter::GameLayer *gameLayer = new Mochi::Shooter::GameLayer(engine.GetCatalog(), engine.GetScriptingManager(), engine.GetRenderer(), engine.GetCamera(),
                                                                         engine.GetEventBus(), engine.GetGUI(), engine.GetActionManager());
    engine.PushLayer(gameLayer);

    engine.Run();
}

#ifdef _MSC_VER
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return main();
}
#endif
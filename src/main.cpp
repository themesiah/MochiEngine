#ifdef _MSC_VER
#include <Windows.h>
#endif

#include "sol/optional_implementation.hpp"
#if DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include "Engine.h"
#include "GameLayer.h"
#include <ostream>

int main()
{
#if DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
#endif
    const char *appName = "Space Shooter";
    const char *appVersion = "0.1";
    const char *appId = "com.scarletmochi.spaceshooter";
    const char *windowName = "Space Shooter";
    Mochi::Engine engine(appName, appVersion, appId, windowName);
    Mochi::Shooter::GameLayer *gameLayer = new Mochi::Shooter::GameLayer();
    engine.PushLayer(gameLayer);

#if DEBUG
    //_CrtSetBreakAlloc(12638);
    //_CrtSetBreakAlloc(12637);
    //_CrtSetBreakAlloc(10458);
#endif

    engine.Run();
    return 0;
}

#ifdef _MSC_VER
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return main();
}
#endif
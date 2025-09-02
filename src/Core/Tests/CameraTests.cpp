// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../Graphics/Camera.h"
#include <SDL3/SDL.h>

TEST_CASE("Camera init")
{
    SDL_FPoint cameraPos;
    cameraPos.x = 0;
    cameraPos.y = 0;
    Camera cam(cameraPos, 1);
    CHECK(cam.GetPosition().x == 0);
    CHECK(cam.GetPosition().y == 0);
    CHECK(cam.GetZoom() == 1);
}

TEST_CASE("World and screen")
{
    SDL_FPoint cameraPos;
    cameraPos.x = 0;
    cameraPos.y = 0;
    Camera cam(cameraPos, 1);

    SDL_FRect destRect;
    destRect.x = 0;
    destRect.y = 0;
    destRect.w = 50;
    destRect.h = 50;

    auto result = cam.WorldToScreen(destRect);
    CHECK_EQ(-result.x, 25);
    CHECK_EQ(-result.y, 25);

    auto result2 = cam.ScreenToWorld(result);
    CHECK_EQ(result2.x, 0);
    CHECK_EQ(result2.y, 0);
}
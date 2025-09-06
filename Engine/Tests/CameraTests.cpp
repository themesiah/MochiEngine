// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../Graphics/Camera.h"
#include <SDL3/SDL.h>

TEST_CASE("Camera init")
{
    Mochi::Graphics::Camera cam((SDL_FPoint){0, 0}, 1, (SDL_Point){1, 1});
    CHECK(cam.GetPosition().x == 0);
    CHECK(cam.GetPosition().y == 0);
    CHECK(cam.GetZoom() == 1);
}

TEST_CASE("World and screen")
{
    Mochi::Graphics::Camera cam((SDL_FPoint){0, 0}, 1, (SDL_Point){1, 1});

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

TEST_CASE("Is on screen")
{
    Mochi::Graphics::Camera cam((SDL_FPoint){0, 0}, 1, (SDL_Point){100, 100});
    SDL_FRect rect1{0, 0, 50, 50};
    SDL_FRect rect2{-1, 0, 50, 50};
    SDL_FRect rect3{-10, 0, 50, 50};
    SDL_FRect rect4{-1, -1, 50, 50};
    SDL_FRect rect5{10, 0, 50, 50};
    SDL_FRect rect6{0, -10, 50, 50};
    SDL_FRect rect7{1, 1, 50, 50};
    SDL_FRect rect8{0, 1, 50, 50};

    CHECK(cam.IsOnScreen(rect1));
    CHECK(cam.IsOnScreen(rect2));
    CHECK_FALSE(cam.IsOnScreen(rect3));
    CHECK(cam.IsOnScreen(rect4));
    CHECK_FALSE(cam.IsOnScreen(rect5));
    CHECK_FALSE(cam.IsOnScreen(rect6));
    CHECK(cam.IsOnScreen(rect7));
    CHECK(cam.IsOnScreen(rect8));
}
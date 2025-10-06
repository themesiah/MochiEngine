// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../Graphics/Camera.h"
#include <SDL3/SDL.h>
#include "Types/Types.hpp"

TEST_CASE("Camera::1- Camera init")
{
    Mochi::Graphics::Camera cam({0, 0}, 1, {1, 1});
    CHECK(cam.GetPosition().x == 0);
    CHECK(cam.GetPosition().y == 0);
    CHECK(cam.GetZoom() == 1);
}

TEST_CASE("Camera::2- World and screen")
{
    Mochi::Graphics::Camera cam({0, 0}, 1, {1, 1});

    Mochi::Rectf destRect;
    destRect.x = 0;
    destRect.y = 0;
    destRect.w = 50;
    destRect.h = 50;

    auto result = cam.WorldToScreen(destRect);
    CHECK_EQ(result.x, -25);
    CHECK_EQ(result.y, -25);

    auto result2 = cam.ScreenToWorld(result);
    CHECK_EQ(result2.x, 0);
    CHECK_EQ(result2.y, 0);
}

TEST_CASE("Camera::3- Is on screen")
{
    Mochi::Graphics::Camera cam({0, 0}, 1, {100, 100});
    Mochi::Rectf rect1{0, 0, 50, 50};
    Mochi::Rectf rect2{-1, 0, 50, 50};
    Mochi::Rectf rect3{-10, 0, 50, 50};
    Mochi::Rectf rect4{-1, -1, 50, 50};
    Mochi::Rectf rect5{10, 0, 50, 50};
    Mochi::Rectf rect6{0, -10, 50, 50};
    Mochi::Rectf rect7{1, 1, 50, 50};
    Mochi::Rectf rect8{0, 1, 50, 50};

    CHECK(cam.IsOnScreen(rect1));
    CHECK(cam.IsOnScreen(rect2));
    CHECK_FALSE(cam.IsOnScreen(rect3));
    CHECK(cam.IsOnScreen(rect4));
    CHECK_FALSE(cam.IsOnScreen(rect5));
    CHECK_FALSE(cam.IsOnScreen(rect6));
    CHECK(cam.IsOnScreen(rect7));
    CHECK(cam.IsOnScreen(rect8));
}

TEST_CASE("Camera::4- World and screen (point)")
{
    Mochi::Graphics::Camera cam({0, 0}, 1, {320, 180});

    Mochi::Vector2f pos;
    pos.x = 0;
    pos.y = 0;

    auto result = cam.WorldToScreen(pos);
    CHECK_EQ(result.x, 160);
    CHECK_EQ(result.y, 90);

    auto result2 = cam.ScreenToWorld(result);
    CHECK_EQ(result2.x, 0);
    CHECK_EQ(result2.y, 0);

    Mochi::Vector2f pos2{1, 2};

    result = cam.WorldToScreen(pos2);
    result2 = cam.ScreenToWorld(result);
    CHECK_EQ(result2.x, 1);
    CHECK_EQ(result2.y, 2);
}
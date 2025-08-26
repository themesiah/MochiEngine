// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Input/InputManager.h"

TEST_CASE("Input Manager")
{
    InputManager input;
    bool keyboardState[SDL_SCANCODE_COUNT];
    for (unsigned int i = 0; i < SDL_SCANCODE_COUNT; ++i)
    {
        keyboardState[i] = false;
    }

    input.Update(keyboardState);
    CHECK_FALSE(input.IsDown(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasPressed(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasReleased(SDL_SCANCODE_A));

    keyboardState[SDL_SCANCODE_A] = true;
    input.Update(keyboardState);
    CHECK(input.IsDown(SDL_SCANCODE_A));
    CHECK(input.WasPressed(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasReleased(SDL_SCANCODE_A));

    keyboardState[SDL_SCANCODE_A] = false;
    input.Update(keyboardState);
    CHECK_FALSE(input.IsDown(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasPressed(SDL_SCANCODE_A));
    CHECK(input.WasReleased(SDL_SCANCODE_A));

    input.Update(keyboardState);
    CHECK_FALSE(input.IsDown(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasPressed(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasReleased(SDL_SCANCODE_A));
}
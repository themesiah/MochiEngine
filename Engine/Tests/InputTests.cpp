// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <memory>

#include "Input/InputManager.h"
#include "Input/ActionManager.h"

TEST_CASE("Input Manager")
{
    Mochi::Input::InputManager input;
    bool keyboardState[SDL_SCANCODE_COUNT];
    memset(keyboardState, false, sizeof(keyboardState));

    input.Update(keyboardState, 0, 0, 0);
    CHECK_FALSE(input.IsDown(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasPressed(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasReleased(SDL_SCANCODE_A));

    keyboardState[SDL_SCANCODE_A] = true;
    input.Update(keyboardState, 0, 0, 0);
    CHECK(input.IsDown(SDL_SCANCODE_A));
    CHECK(input.WasPressed(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasReleased(SDL_SCANCODE_A));

    keyboardState[SDL_SCANCODE_A] = false;
    input.Update(keyboardState, 0, 0, 0);
    CHECK_FALSE(input.IsDown(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasPressed(SDL_SCANCODE_A));
    CHECK(input.WasReleased(SDL_SCANCODE_A));

    input.Update(keyboardState, 0, 0, 0);
    CHECK_FALSE(input.IsDown(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasPressed(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasReleased(SDL_SCANCODE_A));
}

TEST_CASE("Action manager")
{
    std::shared_ptr<Mochi::Input::InputManager> inputManager = std::make_shared<Mochi::Input::InputManager>();
    bool keyboardState[SDL_SCANCODE_COUNT];
    memset(keyboardState, false, sizeof(keyboardState));

    Mochi::Input::ActionManager actionManager(inputManager);
    bool success = actionManager.LoadActionsFromFile("TestData/Actions.json");
    REQUIRE(success);
    REQUIRE(actionManager.HasAction("Shot"));
    REQUIRE(actionManager.HasAction("Horizontal"));

    actionManager.Update(0.1f, keyboardState, 0, 0, 0);
    CHECK_FALSE(actionManager.Performed("Shot"));
    CHECK_FALSE(actionManager.Performed("Horizontal"));

    keyboardState[SDL_SCANCODE_Z] = true;
    actionManager.Update(0.1f, keyboardState, 0, 0, 0);
    CHECK(actionManager.Performed("Shot"));
    CHECK_FALSE(actionManager.Performed("Horizontal"));

    keyboardState[SDL_SCANCODE_LEFT] = true;
    actionManager.Update(0.1f, keyboardState, 0, 0, 0);
    CHECK(actionManager.Performed("Shot"));
    CHECK(actionManager.Performed("Horizontal"));
    CHECK_LT(actionManager.Value("Horizontal"), 0.0f);

    keyboardState[SDL_SCANCODE_RIGHT] = true;
    actionManager.Update(0.1f, keyboardState, 0, 0, 0);
    CHECK(actionManager.Performed("Shot"));
    CHECK_FALSE(actionManager.Performed("Horizontal"));
    CHECK_EQ(actionManager.Value("Horizontal"), 0.0f);

    keyboardState[SDL_SCANCODE_LEFT] = false;
    actionManager.Update(0.1f, keyboardState, 0, 0, 0);
    CHECK(actionManager.Performed("Shot"));
    CHECK(actionManager.Performed("Horizontal"));
    CHECK_GT(actionManager.Value("Horizontal"), 0.0f);

    keyboardState[SDL_SCANCODE_RIGHT] = false;
    keyboardState[SDL_SCANCODE_Z] = false;
    actionManager.Update(0.1f, keyboardState, 0, 0, 0);
    CHECK_FALSE(actionManager.Performed("Shot"));
    CHECK_FALSE(actionManager.Performed("Horizontal"));
    CHECK_EQ(actionManager.Value("Horizontal"), 0.0f);
}
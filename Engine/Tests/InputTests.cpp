// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <memory>

#include "Input/InputManager.h"
#include "Input/ActionManager.h"

#include "Input/IKeyboardProvider.h"
#include "Input/IMouseProvider.h"
#include "Input/IGamepadProvider.h"
#include "Input/InputCommons.h"

#include "Types/Types.hpp"

using namespace Mochi;
using namespace Mochi::Input;

struct MockKeyboardProvider : IKeyboardProvider
{
public:
    bool state[SDL_SCANCODE_COUNT];
    ~MockKeyboardProvider() {}
    virtual const bool *GetState() const { return state; }
    virtual size_t GetMaxKeys() const { return SDL_SCANCODE_COUNT; }
};

struct MockMouseProvider : IMouseProvider
{
public:
    std::array<bool, MouseButton::MouseButtonsCount> state;
    Vector2f position;
    ~MockMouseProvider() {}
    virtual std::array<bool, MouseButton::MouseButtonsCount> GetState(float *x, float *y) const
    {
        *x = position.x;
        *y = position.y;
        return state;
    }
};

struct MockGamepadProvider : IGamepadProvider
{
public:
    std::array<float, GamepadAxis::GAMEPAD_AXIS_COUNT> AxisData;
    std::array<bool, GamepadButton::GAMEPAD_BUTTON_COUNT> ButtonsData;
    ~MockGamepadProvider() {}
    virtual GamepadData GetData(const unsigned int &player) const { return {AxisData, ButtonsData}; }
    virtual bool HasGamepad(const unsigned int &player) const { return player == 0; }
};

TEST_CASE("Input::1- Input Manager")
{
    MockKeyboardProvider *mockKeyboard = new MockKeyboardProvider();
    MockMouseProvider *mockMouse = new MockMouseProvider();
    MockGamepadProvider *mockGamepad = new MockGamepadProvider();
    InputManager input{std::unique_ptr<MockKeyboardProvider>(mockKeyboard), std::unique_ptr<MockMouseProvider>(mockMouse), std::unique_ptr<MockGamepadProvider>(mockGamepad)};
    memset(mockKeyboard->state, false, sizeof(mockKeyboard->state));

    input.Update();
    CHECK_FALSE(input.IsDown(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasPressed(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasReleased(SDL_SCANCODE_A));

    mockKeyboard->state[SDL_SCANCODE_A] = true;
    input.Update();
    CHECK(input.IsDown(SDL_SCANCODE_A));
    CHECK(input.WasPressed(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasReleased(SDL_SCANCODE_A));

    mockKeyboard->state[SDL_SCANCODE_A] = false;
    input.Update();
    CHECK_FALSE(input.IsDown(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasPressed(SDL_SCANCODE_A));
    CHECK(input.WasReleased(SDL_SCANCODE_A));

    input.Update();
    CHECK_FALSE(input.IsDown(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasPressed(SDL_SCANCODE_A));
    CHECK_FALSE(input.WasReleased(SDL_SCANCODE_A));
}

TEST_CASE("Input::2- Action manager")
{
    MockKeyboardProvider *mockKeyboard = new MockKeyboardProvider();
    MockMouseProvider *mockMouse = new MockMouseProvider();
    MockGamepadProvider *mockGamepad = new MockGamepadProvider();
    memset(mockKeyboard->state, false, sizeof(mockKeyboard->state));

    ActionManager actionManager(std::make_unique<InputManager>(std::unique_ptr<MockKeyboardProvider>(mockKeyboard), std::unique_ptr<MockMouseProvider>(mockMouse), std::unique_ptr<MockGamepadProvider>(mockGamepad)));
    bool success = actionManager.LoadActionsFromFile("TestData/Actions.json");
    REQUIRE(success);
    REQUIRE(actionManager.HasAction("Shot"));
    REQUIRE(actionManager.HasAction("Horizontal"));

    actionManager.Update(0.1f);
    CHECK_FALSE(actionManager.Performed("Shot"));

    mockKeyboard->state[SDL_SCANCODE_Z] = true;
    actionManager.Update(0.1f);
    CHECK(actionManager.Performed("Shot"));

    mockKeyboard->state[SDL_SCANCODE_LEFT] = true;
    actionManager.Update(0.1f);
    CHECK(actionManager.Performed("Shot"));
    CHECK_LT(actionManager.Value("Horizontal"), 0.0f);

    mockKeyboard->state[SDL_SCANCODE_RIGHT] = true;
    actionManager.Update(0.1f);
    CHECK(actionManager.Performed("Shot"));
    CHECK_EQ(actionManager.Value("Horizontal"), 0.0f);

    mockKeyboard->state[SDL_SCANCODE_LEFT] = false;
    actionManager.Update(0.1f);
    CHECK(actionManager.Performed("Shot"));
    CHECK_GT(actionManager.Value("Horizontal"), 0.0f);

    mockKeyboard->state[SDL_SCANCODE_RIGHT] = false;
    mockKeyboard->state[SDL_SCANCODE_Z] = false;
    actionManager.Update(0.1f);
    CHECK_FALSE(actionManager.Performed("Shot"));
    CHECK_EQ(actionManager.Value("Horizontal"), 0.0f);

    mockMouse->state = {true, false, false, false, false};
    actionManager.Update(0.1f);
    CHECK(actionManager.Performed("MouseLeftClickDown"));
    mockMouse->state = {true, false, false, false, false};
    actionManager.Update(0.1f);
    CHECK(actionManager.Performed("MouseLeftClickDown"));
    mockMouse->state = {false, false, false, false, false};
    actionManager.Update(0.1f);
    CHECK_FALSE(actionManager.Performed("MouseLeftClickDown"));
}
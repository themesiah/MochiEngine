#include "InputManager.h"

#include <iostream>
#include <SDL3/SDL.h>

#include "../Graphics/Renderer.h" // --> for SDL_RenderCoordinatesFromWindow
#include "../Event/EventBus.h"

#include "../Utils/Logger.h"

#include "IKeyboardProvider.h"
#include "IMouseProvider.h"

#include "../Exception.hpp"

namespace Mochi::Input
{
    InputManager::InputManager(std::shared_ptr<IKeyboardProvider> keyboardProvider, std::shared_ptr<IMouseProvider> mouseProvider) : mKeyboardProvider(keyboardProvider),
                                                                                                                                     mMouseProvider(mouseProvider)
    {
        memset(mKeyboardState, false, sizeof(mKeyboardState));
        memset(mKeyboardLastState, false, sizeof(mKeyboardLastState));
    }

    InputManager::InputManager(std::shared_ptr<Graphics::Renderer> renderer, std::shared_ptr<Event::EventBus> eventBus) : mRenderer(renderer), mEventBus(eventBus)
    {
        memset(mKeyboardState, false, sizeof(mKeyboardState));
        memset(mKeyboardLastState, false, sizeof(mKeyboardLastState));

        mSDLEventSubscriptionHandler = eventBus->Subscribe<SDL_Event>([&](const SDL_Event &e)
                                                                      {
            if (e.type == SDL_EVENT_GAMEPAD_ADDED)
            {
                LOG_INFO(std::format("Gamepad with id {} added!", e.gdevice.which));
            } else if (e.type == SDL_EVENT_GAMEPAD_REMOVED)
            {
                LOG_INFO(std::format("Gamepad with id {} removed!", e.gdevice.which));
            } else {
                //LOG_INFO(std::format("Other event: {}", e.type));
            } });
    }

    InputManager::~InputManager()
    {
        // mEventBus->Unsubscribe<SDL_Event>(mSDLEventSubscriptionHandler);
    }

    bool InputManager::IsDown(const int &key) const
    {
        return mKeyboardState[key];
    }

    bool InputManager::WasPressed(const int &key) const
    {
        return mKeyboardState[key] && !mKeyboardLastState[key];
    }

    bool InputManager::WasReleased(const int &key) const
    {
        return !mKeyboardState[key] && mKeyboardLastState[key];
    }

    void InputManager::Update()
    {
        // Keyboard
        const bool *keyboardState = mKeyboardProvider->GetState();
        const size_t keyCount = mKeyboardProvider->GetMaxKeys();
        memcpy(mKeyboardLastState, mKeyboardState, keyCount);
        memcpy(mKeyboardState, keyboardState, keyCount);

        // Mouse
        float x, y;
        auto newState = mMouseProvider->GetState(&x, &y);
        mMouseLastState = mMouseState;
        mMouseState = newState;
        mDeltaMouseX = x - mMouseX;
        mDeltaMouseY = y - mMouseY;
        mMouseX = x;
        mMouseY = y;

        // Gamepad
        // SDL_Gamepad *gamepad = SDL_GetGamepadFromPlayerIndex(0);
        // SDL_GamepadAxis axis = SDL_GetGamepadAxisFromString("asd");
        // SDL_GetGamepadAxis(gamepad, axis);
    }

    bool InputManager::MouseIsDown(const int &mouseButton) const
    {
        if (mouseButton >= mMouseState.size())
            throw EngineError(std::format("Mouse buttons are {} at max", mMouseState.size()));
        return mMouseState[mouseButton];
    }

    bool InputManager::MouseWasPressed(const int &mouseButton) const
    {
        if (mouseButton >= mMouseState.size())
            throw EngineError(std::format("Mouse buttons are {} at max", mMouseState.size()));
        return mMouseState[mouseButton] && !mMouseLastState[mouseButton];
    }

    bool InputManager::MouseWasReleased(const int &mouseButton) const
    {
        if (mouseButton >= mMouseState.size())
            throw EngineError(std::format("Mouse buttons are {} at max", mMouseState.size()));
        return !mMouseState[mouseButton] && mMouseLastState[mouseButton];
    }

    Vector2f InputManager::GetMousePosition() const
    {
        return {mMouseX, mMouseY};
    }

    Vector2f InputManager::GetMouseDelta() const
    {
        return {mDeltaMouseX, mDeltaMouseY};
    }

}

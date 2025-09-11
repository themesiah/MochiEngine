#include "SDLGamepadProvider.h"

#include <SDL3/SDL.h>

#include "../Utils/Logger.h"
#include "../Exception.hpp"

namespace Mochi::Input
{
    SDLGamepadProvider::SDLGamepadProvider(std::shared_ptr<Event::EventBus> eventBus) : mEventBus(eventBus), mSDLEventSubscriptionHandler(0)
    {
        mSDLEventSubscriptionHandler = mEventBus->Subscribe<SDL_Event>([&](const SDL_Event &e)
                                                                       {
            if (e.type == SDL_EVENT_GAMEPAD_ADDED)
            {
                LOG_INFO(std::format("Gamepad with id {} added", e.gdevice.which));
                auto gamepad = SDL_OpenGamepad(e.gdevice.which);
                mGamepads.push_back(std::shared_ptr<SDL_Gamepad>(gamepad, SDL_CloseGamepad));
            } else if (e.type == SDL_EVENT_GAMEPAD_REMOVED)
            {
                LOG_INFO(std::format("Gamepad with id {} removed", e.gdevice.which));
                mGamepads.erase(
                    std::remove_if(mGamepads.begin(), mGamepads.end(),
                        [&](std::shared_ptr<SDL_Gamepad> gamepad){ return SDL_GetGamepadID(gamepad.get()) == e.gdevice.which; }),
                    mGamepads.end()
                );
        } });
    }

    SDLGamepadProvider::~SDLGamepadProvider()
    {
        if (mSDLEventSubscriptionHandler != 0)
        {
            mEventBus->Unsubscribe<SDL_Event>(mSDLEventSubscriptionHandler);
        }
    }

    GamepadData SDLGamepadProvider::GetData(const unsigned int &player) const
    {
        if (player >= mGamepads.size())
            throw EngineError("There is no gamepad for that player");
        auto playerGamepad = mGamepads[player].get();

        std::array<float, SDL_GamepadAxis::SDL_GAMEPAD_AXIS_COUNT> axisData;
        for (size_t i = 0; i < axisData.size(); i++)
        {
            int16_t raw = SDL_GetGamepadAxis(playerGamepad, (SDL_GamepadAxis)i);
            switch (i)
            {
            case SDL_GamepadAxis::SDL_GAMEPAD_AXIS_LEFT_TRIGGER:
            case SDL_GamepadAxis::SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:
            // Triggers are between 0 and 32767
            case SDL_GamepadAxis::SDL_GAMEPAD_AXIS_LEFTX:
            case SDL_GamepadAxis::SDL_GAMEPAD_AXIS_RIGHTX:
                // Horizontal axis is between -32768 and 32767 (left and right)
                axisData[i] = (float)raw / 32767.0f;
                break;
            case SDL_GamepadAxis::SDL_GAMEPAD_AXIS_LEFTY:
            case SDL_GamepadAxis::SDL_GAMEPAD_AXIS_RIGHTY:
                // Vertical axis is between -32768 and 32767 (up and down)
                axisData[i] = (float)-raw / 32767.0f;
                break;
            }
            if (axisData[i] > 1.0f)
                axisData[i] = 1.0f;
        }

        std::array<bool, SDL_GamepadButton::SDL_GAMEPAD_BUTTON_COUNT> buttonsData;
        for (size_t i = 0; i < buttonsData.size(); i++)
        {
            buttonsData[i] = SDL_GetGamepadButton(playerGamepad, (SDL_GamepadButton)i);
        }

        GamepadData data{axisData, buttonsData};
        return data;
    }

    bool SDLGamepadProvider::HasGamepad(const unsigned int &player) const
    {
        return player < mGamepads.size();
    }
}
#ifndef HDEF_SDLGAMEPADPROVIDER
#define HDEF_SDLGAMEPADPROVIDER

#include "IGamepadProvider.h"

#include <memory>
#include <vector>
#include <SDL3/SDL.h>

#include "../Event/EventBus.h"

namespace Mochi::Input
{
    /**
     * @brief Implementation of IGamepadProvider that uses SDL_GetGamepadAxis and SDL_GetGamepadButton to retrieve gamepad data.
     */
    class SDLGamepadProvider : public IGamepadProvider
    {
    private:
        Event::EventBus *mEventBus;
        Event::SubscriptionHandler mGamepadAddedHandler;
        Event::SubscriptionHandler mGamepadRemovedHandler;
        std::vector<std::shared_ptr<SDL_Gamepad>> mGamepads;

    public:
        SDLGamepadProvider(Event::EventBus *eventBus);
        virtual ~SDLGamepadProvider();
        virtual GamepadData GetData(const unsigned int &player) const override;
        virtual bool HasGamepad(const unsigned int &player) const override;
    };
}

#endif
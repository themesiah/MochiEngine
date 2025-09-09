#ifndef HDEF_INPUTMANAGER
#define HDEF_INPUTMANAGER

#include <memory>

#include "../Types/Types.hpp"

#include "../Event/EventBus.h"

namespace Mochi::Graphics
{
    class Renderer;
}
namespace Mochi::Input
{
    enum MouseButtons
    {
        MouseButtonLeft = 0,
        MouseButtonCenter = 1,
        MouseButtonRight = 2,
        MouseButtonSide1 = 3,
        MouseButtonSide2 = 4
    };
    class IKeyboardProvider;
    class IMouseProvider;
    class IGamepadProvider;
    class InputManager
    {
    private:
        std::shared_ptr<IKeyboardProvider> mKeyboardProvider;
        std::shared_ptr<IMouseProvider> mMouseProvider;

        std::shared_ptr<Graphics::Renderer> mRenderer;
        std::shared_ptr<Event::EventBus> mEventBus;
        Event::SubscriptionHandler mSDLEventSubscriptionHandler;

        bool mKeyboardState[SDL_SCANCODE_COUNT];
        bool mKeyboardLastState[SDL_SCANCODE_COUNT];
        std::array<bool, 5> mMouseState;
        std::array<bool, 5> mMouseLastState;

        float mMouseX;
        float mMouseY;
        float mDeltaMouseX;
        float mDeltaMouseY;

    public:
        InputManager(std::shared_ptr<IKeyboardProvider> keyboardProvider, std::shared_ptr<IMouseProvider> mouseProvider);
        InputManager(std::shared_ptr<Graphics::Renderer> renderer, std::shared_ptr<Event::EventBus> eventBus);
        virtual ~InputManager();
        bool IsDown(const int &key) const;
        bool WasPressed(const int &key) const;
        bool WasReleased(const int &key) const;
        void Update();
        bool MouseIsDown(const int &mouseButton) const;
        bool MouseWasPressed(const int &mouseButton) const;
        bool MouseWasReleased(const int &mouseButton) const;
        Vector2f GetMousePosition() const;
        Vector2f GetMouseDelta() const;
    };
}

#endif
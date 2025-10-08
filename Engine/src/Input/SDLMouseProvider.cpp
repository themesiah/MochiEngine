#include "SDLMouseProvider.h"

#include "SDL3/SDL.h"

#include "../Graphics/IRenderer.h"
#include "../Graphics/SDL/SDLRenderer.h"

namespace Mochi::Input
{
    SDLMouseProvider::SDLMouseProvider(Graphics::IRenderer *renderer)
    {
        auto ren = dynamic_cast<Graphics::SDLRenderer *>(renderer);
        mRenderer = ren->GetRenderer();
    }

    SDLMouseProvider::~SDLMouseProvider()
    {
    }

    std::array<bool, MouseButton::MouseButtonsCount> SDLMouseProvider::GetState(float *x, float *y) const
    {
        const SDL_MouseButtonFlags mouseFlags = SDL_GetMouseState(x, y);
        SDL_RenderCoordinatesFromWindow(mRenderer, *x, *y, x, y);
        std::array<bool, 5> buttons{};
        for (int i = 0; i < 5; ++i)
        {
            buttons[i] = SDL_BUTTON_MASK(i + 1) == mouseFlags;
        }
        return buttons;
    }

}
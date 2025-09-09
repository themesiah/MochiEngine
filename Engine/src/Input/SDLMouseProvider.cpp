#include "SDLMouseProvider.h"

#include <SDL3/SDL.h>
#include "../Graphics/Renderer.h"

namespace Mochi::Input
{
    SDLMouseProvider::SDLMouseProvider(std::shared_ptr<Graphics::Renderer> renderer) : mRenderer(renderer)
    {
    }

    SDLMouseProvider::~SDLMouseProvider()
    {
    }

    std::array<bool, 5> SDLMouseProvider::GetState(float *x, float *y)
    {
        const SDL_MouseButtonFlags mouseFlags = SDL_GetMouseState(x, y);
        SDL_RenderCoordinatesFromWindow(mRenderer->GetRenderer().get(), *x, *y, x, y);
        std::array<bool, 5> buttons{};
        for (int i = 0; i < 5; ++i)
        {
            buttons[i] = SDL_BUTTON_MASK(i + 1) == mouseFlags;
        }
        return buttons;
    }

}
#ifndef HDEF_SDLMOUSEPROVIDER
#define HDEF_SDLMOUSEPROVIDER

#include "IMouseProvider.h"
#include "InputCommons.h"

namespace Mochi::Graphics
{
    class IRenderer;
}
struct SDL_Renderer;
namespace Mochi::Input
{
    /**
     * @brief Implementation of IMouseProvider that uses SDL_GetMouseState and SDL_RenderCoordinatesFromWindow to retrieve mouse data.
     */
    class SDLMouseProvider : public IMouseProvider
    {
    private:
        SDL_Renderer *mRenderer;

    public:
        SDLMouseProvider(const Graphics::IRenderer *renderer);
        virtual ~SDLMouseProvider();
        virtual std::array<bool, MouseButton::MouseButtonsCount> GetState(float *x, float *y) const override;
    };
}

#endif
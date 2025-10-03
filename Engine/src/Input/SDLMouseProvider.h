#ifndef HDEF_SDLMOUSEPROVIDER
#define HDEF_SDLMOUSEPROVIDER

#include "IMouseProvider.h"
#include "InputCommons.h"

namespace Mochi::Graphics
{
    class Renderer;
}
namespace Mochi::Input
{
    class SDLMouseProvider : public IMouseProvider
    {
    private:
        Graphics::Renderer *mRenderer;

    public:
        SDLMouseProvider(Graphics::Renderer *renderer);
        ~SDLMouseProvider();
        virtual std::array<bool, MouseButton::MouseButtonsCount> GetState(float *x, float *y) const;
    };
}

#endif
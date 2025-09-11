#ifndef HDEF_SDLMOUSEPROVIDER
#define HDEF_SDLMOUSEPROVIDER

#include "IMouseProvider.h"
#include "InputCommons.h"

#include <memory>

namespace Mochi::Graphics
{
    class Renderer;
}
namespace Mochi::Input
{
    class SDLMouseProvider : public IMouseProvider
    {
    private:
        std::shared_ptr<Graphics::Renderer> mRenderer;

    public:
        SDLMouseProvider(std::shared_ptr<Graphics::Renderer> renderer);
        ~SDLMouseProvider();
        virtual std::array<bool, MouseButton::MouseButtonsCount> GetState(float *x, float *y) const;
    };
}

#endif
#ifndef HDEF_SDLMOUSEPROVIDER
#define HDEF_SDLMOUSEPROVIDER

#include "IMouseProvider.h"

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
        virtual std::array<bool, 5> GetState(float *x, float *y);
    };
}

#endif
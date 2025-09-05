#ifndef HDEF_IRENDERABLE
#define HDEF_IRENDERABLE

#include "../Graphics/RenderCommand.h"

namespace Mochi
{
    struct IRenderable
    {
        virtual ~IRenderable() {}
        virtual Graphics::RenderCommand GetRenderData() const = 0;
    };
}
#endif
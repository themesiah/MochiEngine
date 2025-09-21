#ifndef HDEF_IRENDERABLE
#define HDEF_IRENDERABLE

#include <vector>
#include "../Graphics/RenderCommand.h"

namespace Mochi
{
    struct IRenderable
    {
        virtual ~IRenderable() {}
        virtual std::vector<Graphics::RenderCommand> GetRenderData() const = 0;
    };
}
#endif
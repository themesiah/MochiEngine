#ifndef HDEF_IRENDERABLE
#define HDEF_IRENDERABLE

#include "../Graphics/RenderCommand.h"

struct IRenderable
{
    virtual ~IRenderable() {}
    virtual RenderCommand GetRenderData() const = 0;
};

#endif
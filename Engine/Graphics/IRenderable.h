#ifndef HDEF_IRENDERABLE
#define HDEF_IRENDERABLE

#include "RenderCommand.h"

class SDL_Renderer;
class IRenderable
{
public:
    virtual ~IRenderable() {}
    virtual RenderCommand GetRenderData() const = 0;
};

#endif
#ifndef HDEF_IRENDERABLE
#define HDEF_IRENDERABLE

class SDL_Renderer;
class IRenderable
{
public:
    virtual ~IRenderable() {}
    virtual void Render(SDL_Renderer *renderer) const = 0;
};

#endif
#ifndef HDEF_SDLGIZMOS
#define HDEF_SDLGIZMOS

#include "IGizmos.h"

namespace Mochi::Graphics
{
    class SDLRenderer;
}
namespace Mochi::Debug
{
    class SDLGizmos : public IGizmos
    {
    private:
        friend class Graphics::SDLRenderer;
        Graphics::SDLRenderer *mRenderer;
        SDLGizmos(Graphics::SDLRenderer *renderer);

    public:
        virtual void DrawRectangle(const Physics::Rectangle *rectangle, const SDL_Color &color) const override;
        virtual void DrawCircle(const Physics::Circle *circle, const SDL_Color &color) const override;
        virtual void DrawLine(const Physics::Line *line, const SDL_Color &color) const override;
        virtual void DrawPoint(const Physics::Point *point, const SDL_Color &color) const override;
    };
}

#endif
#ifndef HDEF_SDLGIZMOS
#define HDEF_SDLGIZMOS

#include "IGizmos.h"

namespace Mochi::Graphics
{
    class SDLRenderer;
}
namespace Mochi
{
    struct Color;
}
namespace Mochi::Debug
{
    /**
     * @brief Implementation of IGizmos using SDL primitive methods for drawing shapes.
     */
    class SDLGizmos : public IGizmos
    {
    private:
        friend class Graphics::SDLRenderer;
        Graphics::SDLRenderer *mRenderer;
        SDLGizmos(Graphics::SDLRenderer *renderer);

    public:
        virtual void DrawRectangle(const Physics::Rectangle *rectangle, const Color &color) const override;
        virtual void DrawCircle(const Physics::Circle *circle, const Color &color) const override;
        virtual void DrawLine(const Physics::Line *line, const Color &color) const override;
        virtual void DrawPoint(const Physics::Point *point, const Color &color) const override;
    };
}

#endif
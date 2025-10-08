#ifndef HDEF_IGIZMOS
#define HDEF_IGIZMOS

#include "../Physics/Shapes.h"

namespace Mochi
{
    struct Color;
}
namespace Mochi::Debug
{
    class IGizmos
    {
    public:
        virtual ~IGizmos();
        void DrawShape(const Physics::Shape &shape, const Color &color) const;
        virtual void DrawRectangle(const Physics::Rectangle *rectangle, const Color &color) const = 0;
        virtual void DrawCircle(const Physics::Circle *circle, const Color &color) const = 0;
        virtual void DrawLine(const Physics::Line *line, const Color &color) const = 0;
        virtual void DrawPoint(const Physics::Point *point, const Color &color) const = 0;
    };
}

#endif
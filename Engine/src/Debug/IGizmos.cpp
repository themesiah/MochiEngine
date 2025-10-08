#include "IGizmos.h"

namespace Mochi::Debug
{
    IGizmos::~IGizmos() {}

    void IGizmos::DrawShape(const Physics::Shape &shape, const SDL_Color &color) const
    {
        if (auto p = dynamic_cast<const Physics::Point *>(&shape))
        {
            DrawPoint(p, color);
        }
        else if (auto l = dynamic_cast<const Physics::Line *>(&shape))
        {
            DrawLine(l, color);
        }
        else if (auto c = dynamic_cast<const Physics::Circle *>(&shape))
        {
            DrawCircle(c, color);
        }
        else if (auto r = dynamic_cast<const Physics::Rectangle *>(&shape))
        {
            DrawRectangle(r, color);
        }
    }
}
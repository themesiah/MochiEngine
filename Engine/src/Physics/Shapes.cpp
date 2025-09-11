#include <array>

#include "Shapes.h"

#include "../Types/Types.hpp"
#include "Collisions.h"

namespace Mochi::Physics
{
    bool Point::Collides(Point p) const { return CollidesPointAndPoint(*this, p); }
    bool Point::Collides(Line l) const { return CollidesPointAndLine(*this, l); }
    bool Point::Collides(Circle c) const { return CollidesPointAndCircle(*this, c); }
    bool Point::Collides(Rectangle r) const { return CollidesPointAndRectangle(*this, r); }

    bool Line::Collides(Point p) const { return CollidesPointAndLine(p, *this); }
    bool Line::Collides(Line l) const { return CollidesLineAndLine(*this, l); }
    bool Line::Collides(Circle c) const { return CollidesLineAndCircle(*this, c); }
    bool Line::Collides(Rectangle r) const { return CollidesLineAndRectangle(*this, r); }

    bool Circle::Collides(Point p) const { return CollidesPointAndCircle(p, *this); }
    bool Circle::Collides(Line l) const { return CollidesLineAndCircle(l, *this); }
    bool Circle::Collides(Circle c) const { return CollidesCircleAndCircle(*this, c); }
    bool Circle::Collides(Rectangle r) const { return CollidesCircleAndRectangle(*this, r); }

    bool Rectangle::Collides(Point p) const { return CollidesPointAndRectangle(p, *this); }
    bool Rectangle::Collides(Line l) const { return CollidesLineAndRectangle(l, *this); }
    bool Rectangle::Collides(Circle c) const { return CollidesCircleAndRectangle(c, *this); }
    bool Rectangle::Collides(Rectangle r) const { return CollidesRectangleAndRectangle(*this, r); }
}
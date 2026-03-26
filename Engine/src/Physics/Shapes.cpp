#include "Shapes.h"

#include <array>
#include <memory>

#include "../Types/Types.hpp"
#include "Collisions.h"
#include "../Utils/MathUtils.h"

namespace Mochi::Physics
{

    std::unique_ptr<Shape> Point::Clone() const { return std::make_unique<Point>(*this); }
    bool Point::Collides(const Point &p) const { return CollidesPointAndPoint(*this, p); }
    bool Point::Collides(const Line &l) const { return CollidesPointAndLine(*this, l); }
    bool Point::Collides(const Circle &c) const { return CollidesPointAndCircle(*this, c); }
    bool Point::Collides(const Rectangle &r) const { return CollidesPointAndRectangle(*this, r); }
    bool Point::IsColliding(const Shape &other) const { return other.Collides(*this); }
    AABB Point::GetAABB() const { return AABB{Position, Position}; }

    std::unique_ptr<Shape> Line::Clone() const { return std::make_unique<Line>(*this); }
    bool Line::Collides(const Point &p) const { return CollidesPointAndLine(p, *this); }
    bool Line::Collides(const Line &l) const { return CollidesLineAndLine(*this, l); }
    bool Line::Collides(const Circle &c) const { return CollidesLineAndCircle(*this, c); }
    bool Line::Collides(const Rectangle &r) const { return CollidesLineAndRectangle(*this, r); }
    bool Line::IsColliding(const Shape &other) const { return other.Collides(*this); }
    AABB Line::GetAABB() const
    {
        Vector2f min;
        Vector2f max;
        if (Position.x > End.x)
        {
            max.x = Position.x;
            min.x = End.x;
        }
        else
        {
            max.x = End.x;
            min.x = Position.x;
        }
        if (Position.y > End.y)
        {
            max.y = Position.y;
            min.y = End.y;
        }
        else
        {
            max.y = End.y;
            min.y = Position.y;
        }
        return AABB{min, max};
    };

    std::unique_ptr<Shape> Circle::Clone() const { return std::make_unique<Circle>(*this); }
    bool Circle::Collides(const Point &p) const { return CollidesPointAndCircle(p, *this); }
    bool Circle::Collides(const Line &l) const { return CollidesLineAndCircle(l, *this); }
    bool Circle::Collides(const Circle &c) const { return CollidesCircleAndCircle(*this, c); }
    bool Circle::Collides(const Rectangle &r) const { return CollidesCircleAndRectangle(*this, r); }
    bool Circle::IsColliding(const Shape &other) const { return other.Collides(*this); }
    AABB Circle::GetAABB() const
    {
        return AABB{Position - Vector2f::One * Radius, Position + Vector2f::One * Radius};
    };

    std::unique_ptr<Shape> Rectangle::Clone() const { return std::make_unique<Rectangle>(*this); }
    bool Rectangle::Collides(const Point &p) const { return CollidesPointAndRectangle(p, *this); }
    bool Rectangle::Collides(const Line &l) const { return CollidesLineAndRectangle(l, *this); }
    bool Rectangle::Collides(const Circle &c) const { return CollidesCircleAndRectangle(c, *this); }
    bool Rectangle::Collides(const Rectangle &r) const { return CollidesRectangleAndRectangle(*this, r); }
    bool Rectangle::IsColliding(const Shape &other) const { return other.Collides(*this); }
    AABB Rectangle::GetAABB() const { return AABB{Position - Extents, Position + Extents}; };
}
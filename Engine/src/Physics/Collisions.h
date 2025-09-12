#ifndef HDEF_COLLISIONS
#define HDEF_COLLISIONS

#include "Shapes.h"
#include "../Utils/MathUtils.h"

namespace Mochi::Physics
{
    inline bool CollidesPointAndPoint(const Point &p1, const Point &p2)
    {
        return p1.Position == p2.Position;
    }

    inline bool CollidesPointAndLine(const Point &p, const Line &l)
    {
        float lineLength = (l.End - l.Position).Distance();
        float d1 = (p.Position - l.Position).Distance();
        float d2 = (p.Position - l.End).Distance();
        return Math::Approx(d1 + d2, lineLength);
    }

    inline bool CollidesPointAndCircle(const Point &p, const Circle &c)
    {
        return (p.Position - c.Position).Distance() <= c.Radius;
    }

    inline bool CollidesPointAndRectangle(const Point &p, const Rectangle &r)
    {
        return p.Position.x >= (r.Position.x - r.Extents.x) &&
               p.Position.x <= (r.Position.x + r.Extents.x) &&
               p.Position.y >= (r.Position.y - r.Extents.y) &&
               p.Position.y <= (r.Position.y + r.Extents.y);
    }

    inline bool CollidesLineAndLine(const Line &l1, const Line &l2)
    {
        float x1 = l1.Position.x;
        float x2 = l1.End.x;
        float x3 = l2.Position.x;
        float x4 = l2.End.x;
        float y1 = l1.Position.y;
        float y2 = l1.End.y;
        float y3 = l2.Position.y;
        float y4 = l2.End.y;
        float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
        float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
        if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
        {
            return true;
        }
        else
        {
            Point start(l1.Position, 0, 0);
            Point end(l1.End, 0, 0);
            return l2.Collides(start) || l2.Collides(end);
        }
    }

    inline bool CollidesLineAndCircle(const Line &l, const Circle &c)
    {
        auto lStart = Point(l.Position, 0, 0);
        auto lEnd = Point(l.End, 0, 0);
        if (c.Collides(lStart) || c.Collides(lEnd))
            return true;

        auto lineVector = l.End - l.Position;
        float lineLength = lineVector.Distance();
        float dot = Vector2f::Dot(c.Position - l.Position, lineVector) / (lineLength * lineLength);
        Point closestPoint = Point(l.Position + (lineVector * dot), 0, 0);
        return c.Collides(closestPoint);
    }

    inline bool CollidesLineAndRectangle(const Line &l, const Rectangle &r)
    {
        Line other = Line(r.Position + Vector2f(r.Extents.x, r.Extents.y), 0, 0, r.Position + Vector2f(r.Extents.x, -r.Extents.y));
        if (l.Collides(other))
            return true;
        other = Line(r.Position + Vector2f(-r.Extents.x, r.Extents.y), 0, 0, r.Position + Vector2f(r.Extents.x, r.Extents.y));
        if (l.Collides(other))
            return true;
        other = Line(r.Position + Vector2f(-r.Extents.x, r.Extents.y), 0, 0, r.Position + Vector2f(-r.Extents.x, -r.Extents.y));
        if (l.Collides(other))
            return true;
        other = Line(r.Position + Vector2f(-r.Extents.x, -r.Extents.y), 0, 0, r.Position + Vector2f(r.Extents.x, -r.Extents.y));
        if (l.Collides(other))
            return true;

        return false;
    }

    inline bool CollidesCircleAndCircle(const Circle &c1, const Circle &c2)
    {
        return (c1.Position - c2.Position).Distance() < c1.Radius + c2.Radius;
    }

    inline bool CollidesCircleAndRectangle(const Circle &c, const Rectangle &r)
    {
        Vector2f test = c.Position;

        if (c.Position.x < r.Position.x - r.Extents.x)
            test.x = r.Position.x - r.Extents.x;
        else if (c.Position.x > r.Position.x + r.Extents.x)
            test.x = r.Position.x + r.Extents.x;

        if (c.Position.y < r.Position.y - r.Extents.y)
            test.y = r.Position.y - r.Extents.y;
        else if (c.Position.y > r.Position.y + r.Extents.y)
            test.y = r.Position.y + r.Extents.y;

        float distance = (c.Position - test).Distance();
        return distance <= c.Radius;
    }

    inline bool CollidesRectangleAndRectangle(const Rectangle &r1, const Rectangle &r2)
    {
        return (r1.Position.x + r1.Extents.x >= r2.Position.x - r2.Extents.x &&
                r1.Position.x - r1.Extents.x <= r2.Position.x + r2.Extents.x &&
                r1.Position.y + r1.Extents.y >= r2.Position.y - r2.Extents.y &&
                r1.Position.y - r1.Extents.y <= r2.Position.y + r2.Extents.y);
    }
}

#endif
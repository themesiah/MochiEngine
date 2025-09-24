#ifndef HDEF_SHAPES
#define HDEF_SHAPES

#include <array>

#include "../Types/Types.hpp"
#include "../Exception.hpp"

namespace Mochi::Physics
{
    struct Point;
    struct Line;
    struct Circle;
    struct Rectangle;
    struct Shape
    {
    protected:
    public:
        Shape(const Vector2f &position) : Position(position) {}
        virtual ~Shape() {}
        virtual bool Collides(Point p) const = 0;
        virtual bool Collides(Line l) const = 0;
        virtual bool Collides(Circle c) const = 0;
        virtual bool Collides(Rectangle r) const = 0;
        Vector2f Position;
    };

    struct Point : public Shape
    {
    public:
        Point(const Vector2f &position) : Shape(position) {}
        virtual ~Point() {}
        virtual bool Collides(Point p) const;
        virtual bool Collides(Line l) const;
        virtual bool Collides(Circle c) const;
        virtual bool Collides(Rectangle r) const;
    };

    struct Line : public Shape
    {
    private:
    public:
        Line(const Vector2f &position, const Vector2f &end) : Shape(position), End(end) {}
        virtual ~Line() {}
        virtual bool Collides(Point p) const;
        virtual bool Collides(Line l) const;
        virtual bool Collides(Circle c) const;
        virtual bool Collides(Rectangle r) const;
        Vector2f End;
    };

    struct Circle : public Shape
    {
    public:
        Circle(const Vector2f &position, const float &radius) : Shape(position), Radius(radius)
        {
            if (Radius <= 0.0f)
                throw EngineError("Circles require a radius greater than 0");
        }
        virtual ~Circle() {}
        virtual bool Collides(Point p) const;
        virtual bool Collides(Line l) const;
        virtual bool Collides(Circle c) const;
        virtual bool Collides(Rectangle r) const;
        float Radius;
    };

    struct Rectangle : public Shape
    {
    private:
    public:
        Rectangle(const Vector2f &position, const Vector2f &extents) : Shape(position), Extents(extents) {}
        virtual ~Rectangle() {}
        virtual bool Collides(Point p) const;
        virtual bool Collides(Line l) const;
        virtual bool Collides(Circle c) const;
        virtual bool Collides(Rectangle r) const;
        Vector2f Extents;
    };
}

#endif
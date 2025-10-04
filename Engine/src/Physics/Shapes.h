#ifndef HDEF_SHAPES
#define HDEF_SHAPES

#include <array>
#include <memory>

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
        virtual std::unique_ptr<Shape> Clone() const = 0;
        virtual bool IsColliding(const Shape &other) const = 0;
        virtual bool Collides(const Point &p) const = 0;
        virtual bool Collides(const Line &l) const = 0;
        virtual bool Collides(const Circle &c) const = 0;
        virtual bool Collides(const Rectangle &r) const = 0;
        Vector2f Position;
    };

    struct Point : public Shape
    {
    public:
        Point(const Vector2f &position) : Shape(position) {}
        virtual ~Point() {}
        virtual std::unique_ptr<Shape> Clone() const;
        virtual bool IsColliding(const Shape &other) const override;
        virtual bool Collides(const Point &p) const override;
        virtual bool Collides(const Line &l) const override;
        virtual bool Collides(const Circle &c) const override;
        virtual bool Collides(const Rectangle &r) const override;
    };

    struct Line : public Shape
    {
    private:
    public:
        Line(const Vector2f &position, const Vector2f &end) : Shape(position), End(end) {}
        virtual ~Line() {}
        virtual std::unique_ptr<Shape> Clone() const;
        virtual bool IsColliding(const Shape &other) const override;
        virtual bool Collides(const Point &p) const override;
        virtual bool Collides(const Line &l) const override;
        virtual bool Collides(const Circle &c) const override;
        virtual bool Collides(const Rectangle &r) const override;
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
        virtual std::unique_ptr<Shape> Clone() const override;
        virtual bool IsColliding(const Shape &other) const override;
        virtual bool Collides(const Point &p) const override;
        virtual bool Collides(const Line &l) const override;
        virtual bool Collides(const Circle &c) const override;
        virtual bool Collides(const Rectangle &r) const override;
        float Radius;
    };

    struct Rectangle : public Shape
    {
    private:
    public:
        Rectangle(const Vector2f &position, const Vector2f &extents) : Shape(position), Extents(extents) {}
        Rectangle(const Rectf &rect) : Shape(rect.GetPosition()), Extents(rect.GetSize() / 2.0f) {}
        virtual ~Rectangle() {}
        virtual std::unique_ptr<Shape> Clone() const override;
        virtual bool IsColliding(const Shape &other) const override;
        virtual bool Collides(const Point &p) const override;
        virtual bool Collides(const Line &l) const override;
        virtual bool Collides(const Circle &c) const override;
        virtual bool Collides(const Rectangle &r) const override;
        Vector2f Extents;
    };
}

#endif
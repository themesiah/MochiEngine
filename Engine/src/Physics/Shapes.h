#ifndef HDEF_SHAPES
#define HDEF_SHAPES

#include <array>

#include "../Types/Types.hpp"
#include "../Entity/IEntity.h"

namespace Mochi::Physics
{
    using PhysicsLayer = int8_t;
    struct Point;
    struct Line;
    struct Circle;
    struct Rectangle;
    struct Shape
    {
    protected:
    public:
        Shape(const Vector2f &position, const EntityHandler &entity, const PhysicsLayer &layer) : Position(position), Entity(entity), Layer(layer) {}
        virtual ~Shape() {}
        virtual bool Collides(Point p) const = 0;
        virtual bool Collides(Line l) const = 0;
        virtual bool Collides(Circle c) const = 0;
        virtual bool Collides(Rectangle r) const = 0;
        Vector2f Position;
        EntityHandler Entity;
        PhysicsLayer Layer;
    };

    struct Point : public Shape
    {
    public:
        Point(const Vector2f &position, const EntityHandler &entity, const PhysicsLayer &layer) : Shape(position, entity, layer) {}
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
        Line(const Vector2f &position, const EntityHandler &entity, const PhysicsLayer &layer, const Vector2f &end) : Shape(position, entity, layer), End(end) {}
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
        Circle(const Vector2f &position, const EntityHandler &entity, const PhysicsLayer &layer, const float &radius) : Shape(position, entity, layer), Radius(radius) {}
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
        Rectangle(const Vector2f &position, const EntityHandler &entity, const PhysicsLayer &layer, const Vector2f &extents) : Shape(position, entity, layer), Extents(extents) {}
        virtual ~Rectangle() {}
        virtual bool Collides(Point p) const;
        virtual bool Collides(Line l) const;
        virtual bool Collides(Circle c) const;
        virtual bool Collides(Rectangle r) const;
        Vector2f Extents;
    };
}

#endif
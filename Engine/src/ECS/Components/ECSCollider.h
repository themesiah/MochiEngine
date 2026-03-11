#ifndef HDEF_ECSCOLLIDER
#define HDEF_ECSCOLLIDER

#include "../../Physics/Shapes.h"

namespace Mochi::ECS
{
    enum ColliderShapeType
    {
        None,
        Point,
        Line,
        Circle,
        Rectangle
    };

    struct ColliderComponent
    {
        union
        {
            Physics::Point Point;
            Physics::Line Line;
            Physics::Circle Circle;
            Physics::Rectangle Rectangle;
        };
        ColliderShapeType ShapeType;
        uint32_t Layer;
        uint32_t CollisionLayerMask;
        bool Trigger;
        ColliderComponent()
        {
            ShapeType = ColliderShapeType::None;
            Trigger = false;
            Layer = 0;
            CollisionLayerMask = 0;
        }
        ~ColliderComponent() {}
        void Reset(Physics::Point p)
        {
            Point = p;
            ShapeType = ColliderShapeType::Point;
        }
        void Reset(Physics::Line p)
        {
            Line = p;
            ShapeType = ColliderShapeType::Line;
        }
        void Reset(Physics::Circle p)
        {
            Circle = p;
            ShapeType = ColliderShapeType::Circle;
        }
        void Reset(Physics::Rectangle p)
        {
            Rectangle = p;
            ShapeType = ColliderShapeType::Rectangle;
        }
    };

    using CollisionFn = bool (*)(const ColliderComponent &a, const ColliderComponent &b);
    inline bool NoneCollision(const ColliderComponent &a, const ColliderComponent &b)
    {
        return false;
    }
    inline bool PointVSPoint(const ColliderComponent &a, const ColliderComponent &b)
    {
        return a.Point.Collides(b.Point);
    }
    inline bool PointVSLine(const ColliderComponent &a, const ColliderComponent &b)
    {
        return a.Point.Collides(b.Line);
    }
    inline bool PointVSCircle(const ColliderComponent &a, const ColliderComponent &b)
    {
        return a.Point.Collides(b.Circle);
    }
    inline bool PointVSRectangle(const ColliderComponent &a, const ColliderComponent &b)
    {
        return a.Point.Collides(b.Rectangle);
    }
    inline bool LineVSPoint(const ColliderComponent &a, const ColliderComponent &b)
    {
        return PointVSLine(b, a);
    }
    inline bool LineVSLine(const ColliderComponent &a, const ColliderComponent &b)
    {
        return a.Line.Collides(b.Line);
    }
    inline bool LineVSCircle(const ColliderComponent &a, const ColliderComponent &b)
    {
        return a.Line.Collides(b.Circle);
    }
    inline bool LineVSRectangle(const ColliderComponent &a, const ColliderComponent &b)
    {
        return a.Line.Collides(b.Rectangle);
    }
    inline bool CircleVSPoint(const ColliderComponent &a, const ColliderComponent &b)
    {
        return PointVSCircle(b, a);
    }
    inline bool CircleVSLine(const ColliderComponent &a, const ColliderComponent &b)
    {
        return LineVSCircle(b, a);
    }
    inline bool CircleVSCircle(const ColliderComponent &a, const ColliderComponent &b)
    {
        return a.Circle.Collides(b.Circle);
    }
    inline bool CircleVSRectangle(const ColliderComponent &a, const ColliderComponent &b)
    {
        return a.Circle.Collides(b.Rectangle);
    }
    inline bool RectangleVSPoint(const ColliderComponent &a, const ColliderComponent &b)
    {
        return PointVSRectangle(b, a);
    }
    inline bool RectangleVSLine(const ColliderComponent &a, const ColliderComponent &b)
    {
        return LineVSRectangle(b, a);
    }
    inline bool RectangleVSCircle(const ColliderComponent &a, const ColliderComponent &b)
    {
        return CircleVSRectangle(b, a);
    }
    inline bool RectangleVSRectangle(const ColliderComponent &a, const ColliderComponent &b)
    {
        return a.Rectangle.Collides(b.Rectangle);
    }

    CollisionFn CollisionFnMatrix[5][5] = {
        // No shape
        {
            NoneCollision,
            NoneCollision,
            NoneCollision,
            NoneCollision,
            NoneCollision},

        // Point
        {
            NoneCollision,
            PointVSPoint,
            PointVSLine,
            PointVSCircle,
            PointVSRectangle},

        // Line
        {
            NoneCollision,
            LineVSPoint,
            LineVSLine,
            LineVSCircle,
            LineVSRectangle},

        // Circle
        {
            NoneCollision,
            CircleVSPoint,
            CircleVSLine,
            CircleVSCircle,
            CircleVSRectangle},

        // Rectangle
        {
            NoneCollision,
            RectangleVSPoint,
            RectangleVSLine,
            RectangleVSCircle,
            RectangleVSRectangle}};
}

#endif
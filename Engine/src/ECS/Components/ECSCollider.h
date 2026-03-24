#ifndef HDEF_ECSCOLLIDER
#define HDEF_ECSCOLLIDER

#include <variant>

#include "../../Physics/Shapes.h"

namespace Mochi::ECS
{
    using ShapeVariant = std::variant<
        Physics::Point,
        Physics::Line,
        Physics::Circle,
        Physics::Rectangle>;

    struct ColliderComponent
    {
        ShapeVariant Shape;
        uint32_t Layer = 0;
        uint32_t CollisionLayerMask = 0;
        bool Trigger = false;
        ColliderComponent(Physics::Point point, uint32_t layer, uint32_t collisionLayerMask, bool trigger)
            : Shape(point),
              Layer(layer),
              CollisionLayerMask(collisionLayerMask),
              Trigger(trigger) {}
        ColliderComponent(Physics::Line line, uint32_t layer, uint32_t collisionLayerMask, bool trigger)
            : Shape(line),
              Layer(layer),
              CollisionLayerMask(collisionLayerMask),
              Trigger(trigger) {}
        ColliderComponent(Physics::Circle circle, uint32_t layer, uint32_t collisionLayerMask, bool trigger)
            : Shape(circle),
              Layer(layer),
              CollisionLayerMask(collisionLayerMask),
              Trigger(trigger) {}
        ColliderComponent(Physics::Rectangle rect, uint32_t layer, uint32_t collisionLayerMask, bool trigger)
            : Shape(rect),
              Layer(layer),
              CollisionLayerMask(collisionLayerMask),
              Trigger(trigger) {}
        ~ColliderComponent() {}
    };
}

#endif
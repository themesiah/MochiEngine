#ifndef HDEF_TRANSFORM
#define HDEF_TRANSFORM

#include "Types.hpp"

namespace Mochi
{
    class Transform
    {
    private:
        Vector2f mPosition;
        float mScale;

    public:
        Transform() : mPosition({0.0f, 0.0f}), mScale(1.0f) {}
        Vector2f GetPosition() const { return mPosition; }
        void SetPosition(const Vector2f &pos) { mPosition = pos; }
        float GetScale() const { return mScale; }
        void SetScale(const float &scale) { mScale = scale; }
        void Move(const Vector2f &delta) { mPosition += delta; }
    };
}

#endif
#ifndef HDEF_CAMERA
#define HDEF_CAMERA

#include <SDL3/SDL.h>
#include "../Types/Types.hpp"

inline constexpr float CONST_PIXEL_PER_METER = 32.0f;

namespace Mochi::Graphics
{
    class Camera
    {
    private:
        Vector2f mPosition;
        float mZoom;
        SDL_Point mLogicalPresentation;

    public:
        Camera(Vector2f startingPosition, float startingZoom, SDL_Point logicalPresentation);
        ~Camera();
        void SetPosition(const float &x, const float &y);
        void SetPosition(const Vector2f &point);
        void SetZoom(const float &zoom);
        Vector2f GetPosition() const;
        float GetZoom() const;
        void Move(const float &horizontal, const float &vertical);
        Rectf WorldToScreen(const Rectf &dstRect) const;
        Rectf ScreenToWorld(const Rectf &dstRect) const;
        Vector2f WorldToScreen(const Vector2f &position) const;
        Vector2f ScreenToWorld(const Vector2f &position) const;
        bool IsOnScreen(const Rectf &dstRect) const;
        Vector2f GetLogicalPresentation() const;
    };
}

#endif
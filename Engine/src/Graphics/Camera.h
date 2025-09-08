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
        SDL_FRect WorldToScreen(const SDL_FRect &dstRect) const;
        SDL_FRect ScreenToWorld(const SDL_FRect &dstRect) const;
        bool IsOnScreen(const SDL_FRect &dstRect) const;
    };
}

#endif
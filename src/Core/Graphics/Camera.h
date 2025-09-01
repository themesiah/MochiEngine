#ifndef HDEF_CAMERA
#define HDEF_CAMERA

#include <SDL3/SDL.h>

inline constexpr float CONST_PIXEL_PER_METER = 32.0f;

class Camera
{
private:
    SDL_FPoint mPosition;
    float mZoom;

public:
    Camera(SDL_FPoint startingPosition, float startingZoom);
    ~Camera();
    void SetPosition(const float &x, const float &y);
    void SetPosition(const SDL_FPoint &point);
    void SetZoom(const float &zoom);
    SDL_FPoint GetPosition() const;
    float GetZoom() const;
    void Move(const float &horizontal, const float &vertical);
    SDL_FRect WorldToScreen(const SDL_FRect &dstRect) const;
};

#endif
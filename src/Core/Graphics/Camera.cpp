#include "Camera.h"

Camera::Camera(SDL_FPoint startingPosition, float startingZoom) : mPosition(startingPosition), mZoom(startingZoom)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(const float &x, const float &y)
{
    SDL_FPoint point;
    point.x = x;
    point.y = y;
    SetPosition(point);
}

void Camera::SetPosition(const SDL_FPoint &point)
{
    mPosition = point;
}

void Camera::SetZoom(const float &zoom)
{
    mZoom = zoom;
}

SDL_FPoint Camera::GetPosition() const
{
    return mPosition;
}

float Camera::GetZoom() const
{
    return mZoom;
}

void Camera::Move(const float &horizontal, const float &vertical)
{
    mPosition.x += horizontal;
    mPosition.y += vertical;
}

SDL_FRect Camera::WorldToScreen(const SDL_FRect &dstRect) const
{
    SDL_FRect newRect = dstRect;
    newRect.x = (newRect.x - mPosition.x) * CONST_PIXEL_PER_METER - dstRect.w / 2;
    newRect.y = (newRect.y + mPosition.y) * CONST_PIXEL_PER_METER - dstRect.h / 2;
    return newRect;
}
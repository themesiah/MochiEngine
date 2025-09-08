#include "Camera.h"

#include "../Utils/Logger.h"

namespace Mochi::Graphics
{
    Camera::Camera(Vector2f startingPosition, float startingZoom, SDL_Point logicalPresentation) : mPosition(startingPosition), mZoom(startingZoom), mLogicalPresentation(logicalPresentation)
    {
    }

    Camera::~Camera()
    {
    }

    void Camera::SetPosition(const float &x, const float &y)
    {
        Vector2f point;
        point.x = x;
        point.y = y;
        SetPosition(point);
    }

    void Camera::SetPosition(const Vector2f &point)
    {
        mPosition = point;
    }

    void Camera::SetZoom(const float &zoom)
    {
        mZoom = zoom;
    }

    Vector2f Camera::GetPosition() const
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

        newRect.x += mLogicalPresentation.x / 2; // Move local size to camera!
        newRect.y += mLogicalPresentation.y / 2;
        return newRect;
    }

    SDL_FRect Camera::ScreenToWorld(const SDL_FRect &dstRect) const
    {
        SDL_FRect newRect = dstRect;
        newRect.x -= mLogicalPresentation.x / 2;
        newRect.y -= mLogicalPresentation.y / 2;
        newRect.x = (newRect.x + dstRect.w / 2) / CONST_PIXEL_PER_METER + mPosition.x;
        newRect.y = (newRect.y + dstRect.h / 2) / CONST_PIXEL_PER_METER + mPosition.y;
        return newRect;
    }

    bool Camera::IsOnScreen(const SDL_FRect &dstRect) const
    {
        SDL_FRect newRect = WorldToScreen(dstRect);
        LOG_INFO(std::format("Pos was {}x {}y and size was {}w {}h and logical presentation is {}x {}y", newRect.x, newRect.y, newRect.w, newRect.h, mLogicalPresentation.x, mLogicalPresentation.y));

        if (newRect.x + newRect.w < 0)
            return false;
        if (newRect.x > mLogicalPresentation.x)
            return false;
        if (newRect.y + newRect.h < 0)
            return false;
        if (newRect.y > mLogicalPresentation.y)
            return false;
        return true;
    }
}

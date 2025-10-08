#include "Camera.h"

#include <format>

#include "../Utils/Logger.h"
#include "../Constants.h"
#include "../Utils/Conversion.hpp"

namespace Mochi::Graphics
{
    Camera::Camera(Vector2f startingPosition, float startingZoom, Vector2f logicalPresentation) : mPosition(startingPosition), mZoom(startingZoom), mLogicalPresentation(logicalPresentation)
    {
    }

    Camera::~Camera()
    {
    }

    void Camera::SetPosition(const float &x, const float &y)
    {
        SetPosition({x, y});
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

    Rectf Camera::WorldToScreen(const Rectf &dstRect) const
    {
        Rectf newRect = dstRect;
        newRect.x = MetersToPixels(newRect.x - mPosition.x) - dstRect.w / 2;
        newRect.y = MetersToPixels(-newRect.y + mPosition.y) - dstRect.h / 2;

        newRect.x += mLogicalPresentation.x / 2; // Move local size to camera!
        newRect.y += mLogicalPresentation.y / 2;
        return newRect;
    }

    Rectf Camera::ScreenToWorld(const Rectf &dstRect) const
    {
        Rectf newRect = dstRect;
        newRect.x -= mLogicalPresentation.x / 2;
        newRect.y -= mLogicalPresentation.y / 2;
        newRect.x = PixelsToMeters(newRect.x + dstRect.w / 2) + mPosition.x;
        newRect.y = PixelsToMeters(newRect.y + dstRect.h / 2) + mPosition.y;
        return newRect;
    }

    Vector2f Camera::WorldToScreen(const Vector2f &position) const
    {
        Vector2f newPos = position;
        newPos.x = MetersToPixels(newPos.x - mPosition.x);
        newPos.y = MetersToPixels(-newPos.y + mPosition.y);

        newPos.x += mLogicalPresentation.x / 2; // Move local size to camera!
        newPos.y += mLogicalPresentation.y / 2;
        return newPos;
    }

    Vector2f Camera::ScreenToWorld(const Vector2f &position) const
    {
        Vector2f newPos = position;
        newPos.x -= mLogicalPresentation.x / 2;
        newPos.y -= mLogicalPresentation.y / 2;
        newPos.x = PixelsToMeters(newPos.x) + mPosition.x;
        newPos.y = PixelsToMeters(-newPos.y) + mPosition.y;
        return newPos;
    }

    bool Camera::IsOnScreen(const Rectf &dstRect) const
    {
        Rectf newRect = WorldToScreen(dstRect);
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

    Vector2f Camera::GetLogicalPresentation() const
    {
        return {(float)mLogicalPresentation.x, (float)mLogicalPresentation.y};
    }
}

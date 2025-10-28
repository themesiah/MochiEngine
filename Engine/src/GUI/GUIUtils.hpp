#ifndef HDEF_GUIUTILS
#define HDEF_GUIUTILS

#include "../Types/Types.hpp"
#include "../Constants.h"
#include "../Graphics/ITexture.h"

namespace Mochi::Graphics
{

    inline const Vector2f GUI_TOP_LEFT{-1.0f, -1.0f};
    inline const Vector2f GUI_TOP_RIGHT{1.0f, -1.0f};
    inline const Vector2f GUI_TOP_CENTER{0.0f, -1.0f};
    inline const Vector2f GUI_MIDDLE_LEFT{-1.0f, 0.0f};
    inline const Vector2f GUI_MIDDLE_CENTER{0.0f, 0.0f};
    inline const Vector2f GUI_MIDDLE_RIGHT{1.0f, 0.0f};
    inline const Vector2f GUI_BOTTOM_LEFT{-1.0f, 1.0f};
    inline const Vector2f GUI_BOTTOM_CENTER{0.0f, 1.0f};
    inline const Vector2f GUI_BOTTOM_RIGHT{1.0f, 1.0f};

    struct GUIOptions
    {
        Rectf SrcRect{};
        Rectf DstRect{};
        Vector2f ScreenAnchor{GUI_MIDDLE_CENTER};
        Vector2f SpritePivot{GUI_MIDDLE_CENTER};
        Color Color{255, 255, 255, 255};
    };

    struct GUITextOptions
    {
        Rectf DstRect{};
        Vector2f ScreenAnchor{GUI_MIDDLE_CENTER};
        Vector2f SpritePivot{GUI_MIDDLE_CENTER};
        float TextSize{16.0f};
        Color Color{255, 255, 255, 255};
    };

    inline Rectf AnchoredPosition(const Rectf &dstRect, const Vector2f &screenAnchor, const Vector2f &spritePivot)
    {
        Vector2f logical{static_cast<float>(CONST_RENDER_LOGICAL_X), static_cast<float>(CONST_RENDER_LOGICAL_Y)};
        auto elementSize = dstRect.GetSize();
        Vector2f pos = (logical * screenAnchor) / 2.0f + logical / 2.0f;
        pos -= (elementSize / 2.0f) * spritePivot + (elementSize / 2.0f);
        pos += dstRect.GetPosition();

        return {pos, dstRect.GetSize()};
    }
}

#endif
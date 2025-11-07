#ifndef HDEF_GUIUTILS
#define HDEF_GUIUTILS

#include <optional>

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

    inline const Color GUI_DEFAULT_COLOR{255, 255, 255, 255};

    struct GUIOptions
    {
        std::optional<Rectf> ParentRect;
        std::optional<Rectf> SrcRect;
        std::optional<Rectf> DstRect;
        std::optional<Rectf> Slice;
        Vector2f ScreenAnchor;
        Vector2f SpritePivot;
        std::optional<Color> Color;
    };

    struct GUITextOptions
    {
        std::optional<Rectf> ParentRect;
        std::optional<Rectf> DstRect;
        Vector2f ScreenAnchor;
        Vector2f TextPivot;
        float TextSize;
        std::optional<Color> Color;
    };

    struct GUIResult
    {
        Rectf FinalRect;
    };

    struct GUIResultButton : public GUIResult
    {
        bool Pressed;
    };

    inline Rectf AnchoredPosition(const std::optional<Rectf> &parentRect, const Rectf &dstRect, const Vector2f &screenAnchor, const Vector2f &spritePivot)
    {
        Rectf parent;
        if (parentRect)
        {
            parent = parentRect.value();
            parent.SetPosition(parent.GetPosition() + parent.GetSize() / 2.0f);
        }
        else
        {
            Vector2f logical{static_cast<float>(CONST_RENDER_LOGICAL_X), static_cast<float>(CONST_RENDER_LOGICAL_Y)};
            parent = {logical / 2.0f, logical};
        }
        auto elementSize = dstRect.GetSize();
        Vector2f pos = (parent.GetSize() * screenAnchor) / 2.0f + parent.GetPosition();
        pos -= (elementSize / 2.0f) * spritePivot + (elementSize / 2.0f);
        pos += dstRect.GetPosition();

        return {pos, dstRect.GetSize()};
    }
}

#endif
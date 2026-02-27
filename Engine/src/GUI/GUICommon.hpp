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

    inline const Rectf FULL_SCREEN_DST{{0.0f, 0.0f}, {CONST_RENDER_LOGICAL_X, CONST_RENDER_LOGICAL_Y}};

    /// @brief Options used by graphical GUI. This contains all the minimal data to show a sprite, including the path of the texture.
    /// Optional elements of the struct will be defaulted by the GUI implementation if not defined.
    struct GUIOptions
    {
        /// @brief Path or unique identifier of the texture in the catalog.
        std::string TexturePath;
        /// @brief The space occupied by the GUI element that you want to contain this element.
        /// This is useful for putting a text in the center of a button, for example.
        std::optional<Rectf> ParentRect;
        /// @brief Source rect in the used texture, in case it contains more than one sprite.
        std::optional<Rectf> SrcRect;
        /// @brief Destination rect, or where the GUI element will be placed according to the anchor and pivot.
        std::optional<Rectf> DstRect;
        /// @brief 9-slice option to be able to make the sprite bigger than the texture without losing quality.
        std::optional<Rectf> Slice;
        /// @brief Where the origin (0,0) will be when deciding where to place the sprite. Goes from -1,-1 to 1,1.
        Vector2f ScreenAnchor;
        /// @brief Defines the pivot, which will act as the origin of the sprite when placing it in the screen. Goes from -1,-1 to 1,1.
        Vector2f SpritePivot;
        /// @brief Color of the element. This is a blend with the original color of the texture.
        std::optional<Color> Color;
    };

    /// @brief Options used by the graphical GUI specifically for texts. This contains all the minimal data to show text, besides the actual text.
    /// Optional elements of the struct will be defaulted by the GUI implementation if not defined.
    struct GUITextOptions
    {
        /// @brief The space occupied by the GUI element that you want to contain this element.
        /// This is useful for putting a text in the center of a button, for example.
        std::optional<Rectf> ParentRect;
        /// @brief Destination rect, or where the GUI element will be placed according to the anchor and pivot.
        std::optional<Rectf> DstRect;
        /// @brief Where the origin (0,0) will be when deciding where to place the text. Goes from -1,-1 to 1,1.
        Vector2f ScreenAnchor;
        /// @brief Defines the pivot, which will act as the origin of the text when placing it in the screen. Goes from -1,-1 to 1,1.
        Vector2f TextPivot;
        /// @brief Height of the text in pixels.
        float TextSize;
        /// @brief Color of the text.
        std::optional<Color> Color;
    };

    /// @brief The button options is a composite of 3 GUIOptions: the default, the focus and pressed states. It allows to, for example,
    /// make a button smaller when focused, or changing tints between states.
    struct GUIButtonOptions
    {
        GUIOptions BaseOptions;
        GUIOptions FocusedOptions;
        GUIOptions PressedOptions;
    };

    /// @brief Gives the space occupied by the GUI element, to be used as a parent of other GUI elements.
    struct GUIResult
    {
        Rectf FinalRect;
    };

    /// @brief Besides the space occupied by the button to be used as a parent, it gives its current state,
    /// indicating if it is focused (selected or focused by mouse), pressed or released. This way a button can trigger game logic
    /// when being pressed or release without changing the implementation.
    struct GUIResultButton
    {
        Rectf FinalRect;
        bool Focus;
        bool Pressed;
        bool Released;
    };

    /// @brief Gives the actual final position of a GUI element, calculated from parent, anchor, pivot and provided destination.
    /// @param parentRect Optional space occupied by the parent. If not defined, the parent is the full screen.
    /// @param dstRect The desired space to put the GUI on, according to the anchor and pivot.
    /// @param parentAnchor Point of the parent (or screen if no parent) to use as origin of the coordinates space when positioning the element. Goes from -1,-1 to 1,1.
    /// @param spritePivot Point of the element to be used as the origin of the element when positioning it. Goes from -1,-1 to 1,1.
    /// @return The final occupied rect in screen space.
    inline Rectf AnchoredPosition(const std::optional<Rectf> &parentRect, const Rectf &dstRect, const Vector2f &parentAnchor, const Vector2f &spritePivot)
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
        Vector2f pos = (parent.GetSize() * parentAnchor) / 2.0f + parent.GetPosition();
        pos -= (elementSize / 2.0f) * spritePivot + (elementSize / 2.0f);
        pos += dstRect.GetPosition();

        return {pos, dstRect.GetSize()};
    }
}

#endif
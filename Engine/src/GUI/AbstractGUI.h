#ifndef HDEF_ABSTRACTGUI
#define HDEF_ABSTRACTGUI

#include <string>
#include <memory>

#include "../Types/Types.hpp"
#include "GUICommon.hpp"

namespace Mochi
{
    struct Color;
}
namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Input
{
    class IActionManager;
}
namespace Mochi::Graphics
{
    using GUIElementId = int;

    class IRenderer;
    class AbstractTextureFactory;
    /**
     * @brief An abstract class meant to be extended with specific implementations for different renderers.
     * It also provides basic methods called on update to manage navigation in the vertical axis.
     * All overrideable methods on this class are meant to provide ways to implement a simple immediate GUI, not a stateful GUI.
     *
     * @remark This class is mostly provisional. A GUI class can be enormous and have a lot functionality. It is also an Immediate GUI, and
     * arguments can be given to make a stateful GUI instead of a stateless GUI like this. Making a fully fledged UI system was not the point on the engine,
     * and this class and its implementations could be improved when the samples need it.
     *
     * Guarantees:
     *
     * - When implemented, UI shows each frame it is shown.
     *
     * - State of the currently selected UI element is the same while the UI layout don't change and GetNextId is called when needed.
     *
     *
     * Not responsible:
     *
     * - GetNextId should be called inside of the implementation when a element is selectable.
     */
    class AbstractGUI
    {
    private:
        GUIElementId mCurrentId;
        void ResetFrame();

    protected:
        GUIElementId mFocusId;
        GUIElementId mPressedId;
        std::unique_ptr<AbstractTextureFactory> mTextureFactory;
        IRenderer *mRenderer;
        FS::PackCatalog *mCatalog;
        Input::IActionManager *mActionManager;
        /// @brief Use this method withing selectable elements to provide them a unique id while the UI layout remains the same.
        /// If called deterministically, the same selectable elements should always have the same IDs if the layout didn't change.
        /// @return The id of the element.
        GUIElementId GetNextId();
        /// @brief Override this if the new system requires additional data processed each frame.
        virtual void OnUpdate() {}

    public:
        /// @brief
        /// @param catalog Catalog from which to get the UI sprites.
        /// @param renderer Renderer used to draw the images.
        /// @param actionManager Action manager, used to interact with interactable UI elements, like buttons.
        AbstractGUI(FS::PackCatalog *catalog, IRenderer *renderer, Input::IActionManager *actionManager);
        virtual ~AbstractGUI();
        /// @brief Resets the current selectable element count for elements to use GetNextId again this frame. Also checks vertical axis input to move the selected element.
        /// @param dt Delta time
        void Update(const float &dt);

        // UI Elements
        /// @brief Shows a button with a text. It should be able to be navigated and pressed with the mouse.
        /// @param options Options for the button. Check GUIButtonOptions.
        /// @param label Text that the button will show. Use an empty string if you don't want text.
        /// @param textOptions Options for the text. Check GUITextOptions.
        /// @return The state of the button in this frame, including if it is focused, pressed and released.
        virtual GUIResultButton Button(const GUIButtonOptions &options, const char *label, const GUITextOptions &textOptions) { return {{}, false, false}; }
        /// @brief Shows a text.
        /// @param label The text to show.
        /// @param options  Options for the text. Check GUITextOptions.
        /// @return Space occupied by the UI, to be used as parent of other UIs.
        virtual GUIResult Text(const char *label, const GUITextOptions &options) { return {}; }
        /// @brief Shows a sprite.
        /// @param options Options for the sprite. Check GUIOptions.
        /// @return Space occupied by the UI, to be used as parent of other UIs.
        virtual GUIResult Sprite(const GUIOptions &options) { return {}; }
    };
}

#endif
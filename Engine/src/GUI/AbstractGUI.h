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
        GUIElementId GetNextId();
        virtual void OnUpdate() {}

    public:
        AbstractGUI(FS::PackCatalog *catalog, IRenderer *renderer, Input::IActionManager *actionManager);
        virtual ~AbstractGUI();
        void Update(const float &dt);

        // UI Elements
        virtual GUIResultButton Button(const GUIButtonOptions &options, const char *label, const GUITextOptions &textOptions) { return {{}, false, false}; }
        virtual GUIResult Text(const char *label, const GUITextOptions &options) { return {}; }
        virtual GUIResult Sprite(const GUIOptions &options) { return {}; }
    };
}

#endif
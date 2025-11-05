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
    using GUIElementId = unsigned int;

    class IRenderer;
    class AbstractTextureFactory;
    class AbstractGUI
    {
    protected:
        GUIElementId mCurrentId;
        GUIElementId mFocusId;
        std::unique_ptr<AbstractTextureFactory> mTextureFactory;
        IRenderer *mRenderer;
        FS::PackCatalog *mCatalog;
        Input::IActionManager *mActionManager;
        GUIElementId GetNextId();

    public:
        AbstractGUI(FS::PackCatalog *catalog, IRenderer *renderer, Input::IActionManager *actionManager);
        virtual ~AbstractGUI();
        void ResetFrame();

        // UI Elements
        virtual GUIResultButton Button(const std::string &texturePath, const GUIOptions &options, const char *label, const GUITextOptions &textOptions) { return {{}, false}; }
        virtual GUIResult Text(const char *label, const GUITextOptions &options) { return {}; }
        virtual GUIResult Sprite(const std::string &texturePath, const GUIOptions &options) { return {}; }
    };
}

#endif
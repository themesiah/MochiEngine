#ifndef HDEF_SDLGUI
#define HDEF_SDLGUI

#include "AbstractGUI.h"

#include <memory>
#include <vector>

#include "../Types/Types.hpp"
#include "GUICommon.hpp"

struct TTF_Font;
namespace Mochi::Graphics
{
    class SDLRenderer;
    class IRenderer;
    class SDLGUI : public AbstractGUI
    {
    private:
        friend class SDLRenderer;
        std::unique_ptr<TTF_Font, void (*)(TTF_Font *)> mFont;
        std::vector<char> mFontRaw;
        SDLGUI(FS::PackCatalog *catalog, IRenderer *renderer, Input::IActionManager *actionManager);
        SDLRenderer *mSDLRenderer;

    public:
        virtual ~SDLGUI();
        virtual GUIResult Sprite(const GUIOptions &options) override;
        virtual GUIResultButton Button(const GUIButtonOptions &options, const char *label, const GUITextOptions &textOptions) override;
        virtual GUIResult Text(const char *label, const GUITextOptions &options) override;
    };
}

#endif
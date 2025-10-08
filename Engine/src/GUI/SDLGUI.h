#ifndef HDEF_SDLGUI
#define HDEF_SDLGUI

#include "AbstractGUI.h"

#include <memory>

#include "../Types/Types.hpp"

struct TTF_Font;
namespace Mochi::Graphics
{
    class SDLGUI : public AbstractGUI
    {
    private:
        std::unique_ptr<TTF_Font, void (*)(TTF_Font *)> mFont;

    public:
        SDLGUI(FS::PackCatalog *catalog, IRenderer *renderer, Input::IActionManager *actionManager);
        virtual ~SDLGUI();
        virtual bool Button(const char *label, const float &textSize, Rectf dstRect, const std::string &texturePath) override;
        virtual void Text(const char *label, const float &textSize, Vector2f position, const SDL_Color &color) override;
    };
}

#endif
#include "SDLGUI.h"
#include "AbstractGUI.h"

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

#include "../Constants.h"
#include "../Packer/PackCatalog.h"
#include "../Graphics/AbstractTextureFactory.h"
#include "../Graphics/IRenderer.h"
#include "../Graphics/SDL/SDLRenderer.h"
#include "../Graphics/SDL/SDLTexture.h"
#include "../Graphics/SDL/SDLTextureFactory.h"
#include "../Input/IActionManager.h"
#include "../Utils/Assert.h"
#include "../Types/Types.hpp"
#include "GUICommon.hpp"
#include "../Utils/MathUtils.h"

namespace Mochi::Graphics
{
    SDLGUI::SDLGUI(FS::PackCatalog *catalog, IRenderer *renderer, Input::IActionManager *actionManager)
        : AbstractGUI(catalog, renderer, actionManager), mFont({nullptr, TTF_CloseFont})
    {
        mSDLRenderer = static_cast<SDLRenderer *>(renderer);
        if (!TTF_Init())
        {
            throw SystemInitializationError("GUI", SDL_GetError());
        }

        mFontRaw = mCatalog->GetFile(CONST_MAIN_FONT_PATH);
        auto font = TTF_OpenFontIO(SDL_IOFromConstMem(mFontRaw.data(), mFontRaw.size()), true, CONST_DEVBUILD_TEXT_SIZE);
        if (!font)
        {
            throw SystemInitializationError("GUI", SDL_GetError());
        }
        mFont.reset(font);
    }

    SDLGUI::~SDLGUI()
    {
    }

    GUIResult SDLGUI::Sprite(const std::string &texturePath, const GUIOptions &options)
    {
        auto tempOptions = options;
        auto tex = mTextureFactory->GetTexture(texturePath);
        SDLTexture *sdltex = dynamic_cast<SDLTexture *>(tex.get());
        auto texSize = sdltex->GetSize();

        if (!tempOptions.SrcRect)
        {
            tempOptions.SrcRect = {{0.0f, 0.0f}, texSize};
        }
        if (!tempOptions.DstRect)
        {
            tempOptions.DstRect = {{0.0f, 0.0f}, texSize};
        }

        auto finalRect = AnchoredPosition(tempOptions.ParentRect, tempOptions.DstRect.value(), tempOptions.ScreenAnchor, tempOptions.SpritePivot);
        SDL_FRect dst = finalRect;
        SDL_FRect src = tempOptions.SrcRect.value();

        auto const &color = tempOptions.Color.value_or(GUI_DEFAULT_COLOR);
        SDL_SetTextureColorMod(sdltex->GetTexture(), color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(sdltex->GetTexture(), color.a);

        if (tempOptions.Slice.has_value())
        {
            auto const &slice = tempOptions.Slice.value();
            SDL_RenderTexture9Grid(mSDLRenderer->GetRenderer(), sdltex->GetTexture(), &src, slice.x, slice.w, slice.y, slice.h, 1.0f, &dst);
        }
        else
        {
            SDL_RenderTexture(mSDLRenderer->GetRenderer(), sdltex->GetTexture(), &src, &dst);
        }
        return {finalRect};
    }

    GUIResultButton SDLGUI::Button(const std::string &texturePath, const GUIOptions &options, const char *label, const GUITextOptions &textOptions)
    {
        auto guiResult = Sprite(texturePath, options);
        auto tempTextOptions = textOptions;
        tempTextOptions.ParentRect = guiResult.FinalRect;
        Text(label, tempTextOptions);

        if (mActionManager->Performed("UISelect"))
        {
            auto mousePos = mActionManager->CompoundValue("MousePosX", "MousePosY");
            if (mousePos.x > guiResult.FinalRect.x &&
                mousePos.x < guiResult.FinalRect.x + guiResult.FinalRect.w &&
                mousePos.y > guiResult.FinalRect.y &&
                mousePos.y < guiResult.FinalRect.y + guiResult.FinalRect.h)
            {
                return {{guiResult.FinalRect}, true};
            }
        }
        return {{guiResult.FinalRect}, false};
    }

    GUIResult SDLGUI::Text(const char *label, const GUITextOptions &options)
    {
        auto tempOptions = options;
        auto renderer = mSDLRenderer->GetRenderer();
        auto const &color = tempOptions.Color.value_or(GUI_DEFAULT_COLOR);
        SDL_Surface *surface = TTF_RenderText_Solid_Wrapped(mFont.get(), label, 0, color, 0);
        ASSERT(std::format("Surface is null: {}", SDL_GetError()), surface != NULL);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        ASSERT("Texture is null", texture != NULL);
        SDL_SetTextureScaleMode(texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST); // SUPER IMPORTANT!

        float w, h;
        SDL_GetTextureSize(texture, &w, &h);
        // Make the dest rect bigger depending on our text size.
        // Our text size defines the HEIGHT of the text.
        // "h" is the height of the font in this case, and w the width of the full text
        // So we do a factor from our text size and font size and multiply it by the original width
        if (!tempOptions.DstRect)
        {
            tempOptions.DstRect = std::make_optional<Rectf>({});
        }
        tempOptions.DstRect.value().w = w * options.TextSize / h;
        tempOptions.DstRect.value().h = options.TextSize;
        auto finalRect = AnchoredPosition(tempOptions.ParentRect, tempOptions.DstRect.value(), tempOptions.ScreenAnchor, tempOptions.TextPivot);
        SDL_FRect sdlDstRect = finalRect;
        SDL_RenderTexture(renderer, texture, NULL, &sdlDstRect);
        SDL_DestroySurface(surface);
        SDL_DestroyTexture(texture);
        return {finalRect};
    }

}
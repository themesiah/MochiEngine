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
#include "GUIUtils.hpp"
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

    bool SDLGUI::Button(const char *label, const float &textSize, Rectf dstRect, const std::string &texturePath)
    {
        auto tex = mTextureFactory->GetTexture(texturePath);
        SDLTexture *sdltex = dynamic_cast<SDLTexture *>(tex.get());
        if (!sdltex)
        {
            return false;
        }

        auto *ren = dynamic_cast<SDLRenderer *>(mRenderer);

        SDL_FRect dst = dstRect;
        SDL_RenderTexture9Grid(ren->GetRenderer(), sdltex->GetTexture(), NULL, 10, 10, 10, 10, 0, &dst);
        Text(label, textSize, {dstRect.x + dstRect.w / 2 - (textSize * strlen(label) / 4), dstRect.y + dstRect.h / 2 - textSize / 2}, {255, 255, 255, 255});

        if (mActionManager->Performed("UISelect"))
        {
            auto mousePos = mActionManager->CompoundValue("MousePosX", "MousePosY");
            if (mousePos.x > dstRect.x && mousePos.x < dstRect.x + dstRect.w && mousePos.y > dstRect.y && mousePos.y < dstRect.y + dstRect.h)
            {
                return true;
            }
        }

        return false;
    }

    void SDLGUI::Text(const char *label, const float &textSize, Vector2f position, const Color &color)
    {
        auto renderer = mSDLRenderer->GetRenderer();
        SDL_Surface *surface = TTF_RenderText_Solid(mFont.get(), label, 0, color);
        ASSERT(std::format("Surface is null: {}", SDL_GetError()), surface != NULL);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        ASSERT("Texture is null", texture != NULL);
        SDL_SetTextureScaleMode(texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST); // SUPER IMPORTANT!

        float w, h;
        SDL_GetTextureSize(texture, &w, &h);
        SDL_FRect dstRect{position.x, position.y, w / h * textSize, textSize};
        SDL_RenderTexture(renderer, texture, NULL, &dstRect);
        SDL_DestroySurface(surface);
        SDL_DestroyTexture(texture);
    }

    void SDLGUI::Sprite(const std::string &texturePath, const GUIOptions &options)
    {
        auto tempOptions = options;
        auto tex = mTextureFactory->GetTexture(texturePath);
        SDLTexture *sdltex = dynamic_cast<SDLTexture *>(tex.get());
        auto texSize = sdltex->GetSize();

        auto srcSize = tempOptions.SrcRect.GetSize();
        if (Math::Approx(srcSize.x, 0.0f) || Math::Approx(srcSize.y, 0.0f))
        {
            tempOptions.SrcRect.SetSize(texSize);
            tempOptions.SrcRect.SetPosition({0.0f, 0.0f});
        }
        auto dstSize = tempOptions.DstRect.GetSize();
        if (Math::Approx(dstSize.x, 0.0f) || Math::Approx(dstSize.y, 0.0f))
        {
            tempOptions.DstRect.SetSize(tempOptions.SrcRect.GetSize());
        }

        tempOptions.DstRect = AnchoredPosition(tempOptions.DstRect, tempOptions.ScreenAnchor, tempOptions.SpritePivot);
        SDL_FRect dst = tempOptions.DstRect;
        SDL_FRect src = tempOptions.SrcRect;

        SDL_SetTextureColorMod(sdltex->GetTexture(), tempOptions.Color.r, tempOptions.Color.g, tempOptions.Color.b);
        SDL_SetTextureAlphaMod(sdltex->GetTexture(), tempOptions.Color.a);
        SDL_RenderTexture(mSDLRenderer->GetRenderer(), sdltex->GetTexture(), &src, &dst);
    }

}
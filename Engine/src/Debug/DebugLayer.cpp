#include "DebugLayer.h"
#include "../Engine.h"

#include <format>

#include "../Utils/Logger.h"

#include "../Constants.h"

#include "../Graphics/IRenderer.h"
#include "../Graphics/SDL/SDLRenderer.h"
#include "../GUI/AbstractGUI.h"

#include "../Physics/Shapes.h"
#include "IGizmos.h"
#include "../Utils/Conversion.hpp"

#include "../GUI/GUIUtils.hpp"

namespace Mochi
{
    DebugLayer::DebugLayer()
        : Layer()
    {
    }

    DebugLayer::~DebugLayer()
    {
    }

    bool DebugLayer::Update(const float &dt)
    {
        return true;
    }

    void DebugLayer::Render() const
    {
    }

    void DebugLayer::GUI() const
    {
        // Dev build message
        mGUI->Text(CONST_DEVBUILD_TEXT, 16, {0, (float)CONST_RENDER_LOGICAL_Y - 16.0f}, {255, 255, 255, SDL_ALPHA_OPAQUE});
        mGUI->Text(std::format("{} fps", (int)(1.0f / Engine::Get().GetLastRealDelta())).c_str(), 16, {0, 0}, {255, 255, 255, SDL_ALPHA_OPAQUE});

        /*Graphics::GUIOptions options{
            {},                      // src
            {},                      // dst
            Graphics::GUI_TOP_RIGHT, // anchor
            Graphics::GUI_TOP_RIGHT, // pivot
            {255, 255, 255, 255}     // color
        };
        mGUI->Sprite("PlayerLife.png", options);*/
    }

    void DebugLayer::Debug() const
    {
        Physics::Line l({0.0f, -100.0f}, {0.0f, 100.0f});
        mGizmos->DrawLine(&l, {255, 0, 255, 50});
        Physics::Line l2({-100.0f, 0.0f}, {100.0f, 0.0f});
        mGizmos->DrawLine(&l2, {255, 0, 255, 50});
    }
}
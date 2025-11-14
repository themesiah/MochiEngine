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

#include "../GUI/GUICommon.hpp"

namespace Mochi
{
    DebugLayer::DebugLayer()
        : Layer()
    {
    }

    DebugLayer::~DebugLayer()
    {
    }

    void DebugLayer::InitLayer()
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
        const Graphics::GUITextOptions devBuildTextOptions{
            .ScreenAnchor = Graphics::GUI_BOTTOM_RIGHT,
            .TextPivot = Graphics::GUI_BOTTOM_RIGHT,
            .TextSize = 16.0f};
        const Graphics::GUITextOptions fpsTextOptions{
            .ScreenAnchor = Graphics::GUI_TOP_LEFT,
            .TextPivot = Graphics::GUI_TOP_LEFT,
            .TextSize = 16.0f};
        mGUI->Text(CONST_DEVBUILD_TEXT, devBuildTextOptions);
        mGUI->Text(std::format("{} fps", (int)(1.0f / Engine::Get().GetLastRealDelta())).c_str(), fpsTextOptions);
    }

    void DebugLayer::Debug() const
    {
        Physics::Line l({0.0f, -100.0f}, {0.0f, 100.0f});
        mGizmos->DrawLine(&l, {255, 0, 255, 50});
        Physics::Line l2({-100.0f, 0.0f}, {100.0f, 0.0f});
        mGizmos->DrawLine(&l2, {255, 0, 255, 50});
    }
}
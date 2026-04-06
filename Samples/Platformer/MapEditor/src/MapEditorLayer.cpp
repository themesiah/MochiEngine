#include "MapEditorLayer.h"

#include "imgui.h"
#include "imgui_impl_sdlrenderer3.h"
#include "imgui_impl_sdl3.h"

#include "Graphics/SDL/SDLRenderer.h"
#include "Constants.h"
#include "Layer.h"

namespace Mochi::Platformer::Editor
{
    MapEditorLayer::MapEditorLayer() : Layer()
    {
    }

    MapEditorLayer::~MapEditorLayer()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void MapEditorLayer::InitLayer()
    {
        ImGui::CreateContext();
        Graphics::SDLRenderer *renderer = dynamic_cast<Graphics::SDLRenderer *>(mRenderer);

        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(CONST_RENDER_LOGICAL_X, CONST_RENDER_LOGICAL_Y);
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

        ImGui_ImplSDL3_InitForSDLRenderer(renderer->GetWindow(), renderer->GetRenderer());
        ImGui_ImplSDLRenderer3_Init(renderer->GetRenderer());

        mEventPollingSubscription = mEventBus->Subscribe<SDL_Event>([&](const SDL_Event &e)
                                                                    { ImGui_ImplSDL3_ProcessEvent(&e); });
    }

    bool MapEditorLayer::Update(const float &dt)
    {
        return true;
    }

    void MapEditorLayer::Render() const
    {
    }

    void MapEditorLayer::GUI()
    {
        // ImGui_ImplSDL3_ProcessEvent();
        ImGui_ImplSDL3_NewFrame();
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Test");
        ImGui::Text("Holaaa");
        ImGui::End();
        ImGui::Render();
        Graphics::SDLRenderer *renderer = dynamic_cast<Graphics::SDLRenderer *>(mRenderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer->GetRenderer());
    }

#if DEBUG
    void MapEditorLayer::Debug() const
    {
    }
#endif
}
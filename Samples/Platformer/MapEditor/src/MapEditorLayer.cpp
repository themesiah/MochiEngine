#include "MapEditorLayer.h"

#include "imgui.h"
#include "imgui_impl_sdlrenderer3.h"
#include "imgui_impl_sdl3.h"

#include "Graphics/SDL/SDLRenderer.h"
#include "Graphics/AsepriteAnimationFactory.h"
#include "Graphics/AbstractTextureFactory.h"
#include "Constants.h"
#include "Layer.h"

#include "MapEditor.h"

namespace Mochi::Platformer::Editor
{
    MapEditorLayer::MapEditorLayer() : Layer()
    {
        mCatalog->OpenPack("Data/Game");
        mCatalog->OpenPack("Data/EditorData");
        mTextureFactory = mRenderer->CreateTextureFactory(mCatalog);
        mAnimationFactory = std::make_unique<Graphics::AsepriteAnimationFactory>(mCatalog);
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

        mTilemap = std::make_unique<Tilemap>(mECSWorld, mCatalog, mTextureFactory.get(), mAnimationFactory.get());
        mMapEditor = std::make_unique<MapEditor>(mActionManager, mTilemap.get(), mCamera);
        mTilemap->LoadDefault();
        mTilemap->InitMap();
    }

    bool MapEditorLayer::Update(const float &dt)
    {
        if (ImGui::GetIO().WantCaptureMouse)
        {
            mActionManager->SetBlockingLayer(0);
        }
        if (mActionManager->Performed("Drag"))
        {
            Vector2f delta = mActionManager->CompoundValue("DragDeltaHorizontal", "DragDeltaVertical") * dt;
            mCamera->Move(delta.x, delta.y);
        }
        mMapEditor->Update(dt);
        return true;
    }

    void MapEditorLayer::Render() const
    {
        mTilemap->Render();
    }

    void MapEditorLayer::GUI()
    {
    }

#if DEBUG
    void MapEditorLayer::Debug() const
    {
        mTilemap->DebugGizmos(mGizmos, mCamera);

        ImGui_ImplSDL3_NewFrame();
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Map Editor");
        if (ImGui::Button("Reset camera"))
        {
            mCamera->SetPosition(Vector2f::Zero);
        }
        mMapEditor->GUI();
        ImGui::End();
        ImGui::Render();
        Graphics::SDLRenderer *renderer = dynamic_cast<Graphics::SDLRenderer *>(mRenderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer->GetRenderer());
    }
#endif
}
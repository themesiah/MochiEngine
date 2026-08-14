#include "MapEditor.h"

#include "imgui.h"

#include "Input/IActionManager.h"
#include "Graphics/Camera.h"
#include "Utils/Logger.h"
#include "Utils/Conversion.hpp"

#include "Tilemap/Tilemap.h"

namespace Mochi::Platformer::Editor
{
    MapEditor::MapEditor(Input::IActionManager *actionManager, Tilemap *tilemap, Graphics::Camera *camera)
        : mActionManager(actionManager), mTilemap(tilemap), mCamera(camera), mEditingState(MapEditorEditingState::Nothing),
          mSelectedBreakableType(0), mSelectedTileSet(0), mSelectedEnemyType(0)
    {
    }

    MapEditor::~MapEditor()
    {
    }

    void MapEditor::Update(const float &dt)
    {
        bool place = mActionManager->Performed("Place");
        bool erase = mActionManager->Performed("Erase");
        if (place || erase)
        {
            Vector2f mousePos = PixelsToMeters(mActionManager->CompoundValue("MousePosX", "MousePosY"));
            auto camPixels = mCamera->GetPosition();
            mousePos.x += camPixels.x;
            mousePos.y += camPixels.y;
            TilemapTile tile = mTilemap->WorldToTile(mousePos);

            if (mTilemap->IsValidPosition(tile.i, tile.j))
            {
                if (place)
                {
                    PlaceElement(tile.i, tile.j);
                }

                if (erase)
                {
                    RemoveElement(tile.i, tile.j);
                }
            }
        }
    }

    void MapEditor::PlaceElement(const int &i, const int &j)
    {
        switch (mEditingState)
        {
        case MapEditorEditingState::Nothing:
            break;
        case MapEditorEditingState::Tiles:
            mTilemap->AddTile(i, j, mSelectedTileSet);
            break;
        case MapEditorEditingState::Breakables:
            mTilemap->AddBreakable(i, j, mTilemap->GetBreakableIds()[mSelectedBreakableType]);
            break;
        case MapEditorEditingState::Coins:
            mTilemap->AddCoin(i, j);
            break;
        case MapEditorEditingState::Player:
            mTilemap->SetPlayerStartingPosition(i, j);
            break;
        case MapEditorEditingState::Enemies:
            mTilemap->AddEnemy(i, j, mTilemap->GetEnemyIds()[mSelectedEnemyType]);
            break;
        }
    }

    void MapEditor::RemoveElement(const int &i, const int &j)
    {
        switch (mEditingState)
        {
        case MapEditorEditingState::Nothing:
        case MapEditorEditingState::Player:
            break;
        case MapEditorEditingState::Tiles:
        {
            int tileIndex = mTilemap->GetTileIndex(i, j);
            if (tileIndex >= 0)
            {
                mTilemap->RemoveTile(tileIndex);
            }
        }
        break;
        case MapEditorEditingState::Breakables:
        {
            int breakableIndex = mTilemap->GetBreakableIndexAt(i, j);
            if (breakableIndex >= 0)
            {
                mTilemap->RemoveBreakable(breakableIndex);
            }
        }
        break;
        case MapEditorEditingState::Coins:
        {
            int coinIndex = mTilemap->GetCoinIndexAt(i, j);
            if (coinIndex >= 0)
            {
                mTilemap->RemoveCoin(coinIndex);
            }
        }
        break;
        case MapEditorEditingState::Enemies:
        {
            int enemyIndex = mTilemap->GetEnemyIndexAt(i, j);
            if (enemyIndex >= 0)
            {
                mTilemap->RemoveEnemy(enemyIndex);
            }
        }
        break;
        }
    }

    void MapEditor::GUI()
    {
        int editingState = (int)mEditingState;
        ImGui::Combo("Placement type", &editingState, EditingStateItems, IM_ARRAYSIZE(EditingStateItems));
        mEditingState = (MapEditorEditingState)editingState;

        if (mEditingState == MapEditorEditingState::Tiles)
        {
            auto tileSets = mTilemap->GetTilesetIds();
            if (tileSets.size() > 0)
            {
                if (ImGui::BeginCombo("##tileset combo", tileSets[mSelectedTileSet].c_str()))
                {
                    for (int i = 0; i < tileSets.size(); ++i)
                    {
                        bool isSelected = mSelectedTileSet == i;
                        if (ImGui::Selectable(tileSets[i].c_str(), isSelected))
                            mSelectedTileSet = i;
                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }
            else
            {
                ImGui::Text("No tilesets added to tilemap.");
            }
        }
        else if (mEditingState == MapEditorEditingState::Breakables)
        {
            auto breakables = mTilemap->GetBreakableIds();
            if (breakables.size() > 0)
            {
                if (ImGui::BeginCombo("##breakables combo", breakables[mSelectedBreakableType].c_str()))
                {
                    for (int i = 0; i < breakables.size(); ++i)
                    {
                        bool isSelected = mSelectedBreakableType == i;
                        if (ImGui::Selectable(breakables[i].c_str(), isSelected))
                            mSelectedBreakableType = i;
                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }
            else
            {
                ImGui::Text("No breakables added to tilemap.");
            }
        }
        else if (mEditingState == MapEditorEditingState::Enemies)
        {
            auto enemies = mTilemap->GetEnemyIds();
            if (enemies.size() > 0)
            {
                if (ImGui::BeginCombo("##enemies combo", enemies[mSelectedEnemyType].c_str()))
                {
                    for (int i = 0; i < enemies.size(); ++i)
                    {
                        bool isSelected = mSelectedEnemyType == i;
                        if (ImGui::Selectable(enemies[i].c_str(), isSelected))
                            mSelectedEnemyType = i;
                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }
            else
            {
                ImGui::Text("No enemies added to tilemap.");
            }
        }
    }
}
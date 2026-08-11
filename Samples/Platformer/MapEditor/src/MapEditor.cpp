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
          mSelectedBreakableType(""), mSelectedTileSet(0)
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
            mTilemap->AddBreakable(i, j, mSelectedBreakableType);
            break;
        case MapEditorEditingState::Coins:
            mTilemap->AddCoin(i, j);
            break;
        case MapEditorEditingState::Player:
            mTilemap->SetPlayerStartingPosition(i, j);
            break;
        case MapEditorEditingState::Enemies:
            mTilemap->AddEnemy(i, j, mSelectedEnemyType);
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
        ImGui::Text("Holaaa");
        int editingState = (int)mEditingState;
        ImGui::Combo("Placement type", &editingState, EditingStateItems, IM_ARRAYSIZE(EditingStateItems));
        mEditingState = (MapEditorEditingState)editingState;
    }
}
#ifndef HDEF_MAPEDITOR
#define HDEF_MAPEDITOR

#include <string>

namespace Mochi::Platformer
{
    class Tilemap;
}
namespace Mochi::Input
{
    class IActionManager;
}
namespace Mochi::Graphics
{
    class Camera;
}
namespace Mochi::Platformer::Editor
{
    enum MapEditorEditingState
    {
        Nothing,
        Tiles,
        Coins,
        Player,
        Enemies,
        Breakables
    };
    const inline char *EditingStateItems[] = {"Nothing", "Tiles", "Coins", "Player", "Enemies", "Breakables"};

    class MapEditor
    {
    private:
        Input::IActionManager *mActionManager;
        Tilemap *mTilemap;
        Graphics::Camera *mCamera;
        MapEditorEditingState mEditingState;
        std::string mSelectedBreakableType;
        std::string mSelectedEnemyType;
        int mSelectedTileSet;

    public:
        MapEditor(Input::IActionManager *actionManager, Tilemap *tilemap, Graphics::Camera *camera);
        virtual ~MapEditor();
        void Update(const float &dt);
        void PlaceElement(const int &i, const int &j);
        void RemoveElement(const int &i, const int &j);

        void GUI();
    };
}

#endif
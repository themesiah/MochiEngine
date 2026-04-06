#ifndef HDEF_TILE
#define HDEF_TILE

#include "Types/Types.hpp"

namespace Mochi::Platformer
{
    enum SourceType
    {
        Sprite = 0,
        Animation = 1
    };

    struct TilemapProperties
    {
        int Width;
        int Height;
        float TileSize;
    };

    struct EnemyType
    {
        std::string Id;
        SourceType SourceType;
        std::string Source;
        Rectf Collider;
    };

    struct BreakableType
    {
        std::string Id;
        SourceType SourceType;
        std::string Source;
    };

    struct TilemapTile
    {
        int i;
        int j;
        int TilesetIndex;
    };

    struct ZIndexes
    {
        int Scenario;
        int Foreground;
        int Background;
        int Enemies;
        int Coins;
        int Breakables;
        int Player;
    };

    struct TilemapEnemy
    {
        int i;
        int j;
        std::string Type;
    };

    struct TilemapCoin
    {
        int i;
        int j;
    };

    struct TilemapBreakable
    {
        int i;
        int j;
        std::string Type;
    };

    struct TilemapPlayerStart
    {
        int i;
        int j;
    };
}

#endif
#ifndef HDEF_ZINDEXENUM
#define HDEF_ZINDEXENUM

namespace Mochi::Shooter
{
    enum ZIndex
    {
        ZINDEX_BACK = 0,
        ZINDEX_BACKGROUND = 5,

        ZINDEX_PLAYERVFX = 20,
        ZINDEX_ENEMYVFX = 25,

        ZINDEX_PLAYERBULLET = 30,

        ZINDEX_ENEMY = 50,
        ZINDEX_ENEMYBULLET = 70,

        ZINDEX_PLAYER = 80,

        ZINDEX_FOREGROUND = 90,
        ZINDEX_FRONT = 100
    };
}

#endif
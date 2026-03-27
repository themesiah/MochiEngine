#ifndef HDEF_TILEMAP
#define HDEF_TILEMAP

#include "Types/Types.hpp"

namespace Mochi::ECS
{
    class ECSWorld;
}
namespace Mochi::Graphics
{
    class Camera;
}
#if DEBUG
namespace Mochi::Debug
{
    class IGizmos;
}
#endif
namespace Mochi::Platformer
{
    class Tilemap
    {
    private:
        ECS::ECSWorld *mWorld;
        int mWidth;
        int mHeight;
        float mSquareSize;

    public:
        Tilemap(ECS::ECSWorld *world, int width, int height, int squareSize);
        ~Tilemap();
        Rectf GetTile(const int &i, const int &j);
#if DEBUG
        void DebugGizmos(Debug::IGizmos *gizmos) const;
#endif
    };
}

#endif
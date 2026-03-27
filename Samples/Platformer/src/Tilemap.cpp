#include "Tilemap.h"

#include "ECS/ECSWorld.h"
#include "Utils/Logger.h"
#include "Graphics/Camera.h"
#include "Utils/Conversion.hpp"

#if DEBUG
#include "Debug/IGizmos.h"
#endif

namespace Mochi::Platformer
{
    Tilemap::Tilemap(ECS::ECSWorld *world, int width, int height, int squareSize)
        : mWorld(world), mWidth(width), mHeight(height), mSquareSize(PixelsToMeters(float(squareSize)))
    {
    }

    Tilemap::~Tilemap()
    {
    }

    Rectf Tilemap::GetTile(const int &i, const int &j)
    {
        if (i >= mWidth || j >= mHeight)
            throw EngineError("Tilemap is not that big");

        float x = ((float)i - mWidth / 2) * mSquareSize - mSquareSize / 2;
        float y = ((float)j - mHeight / 2) * mSquareSize - mSquareSize / 2;
        return Rectf{x, y, mSquareSize, mSquareSize};
    }

#if DEBUG
    void Tilemap::DebugGizmos(Debug::IGizmos *gizmos) const
    {
        const Color color = Color{255, 255, 0, 30};

        for (int i = -mWidth / 2; i < mWidth / 2; ++i)
        {
            Vector2f start{(float)i * mSquareSize, -mHeight / 2.0f * mSquareSize};
            Vector2f end{(float)i * mSquareSize, mHeight / 2.0f * mSquareSize};
            Physics::Line l{start, end};
            gizmos->DrawLine(&l, color);
        }
        for (int j = -mHeight / 2; j < mHeight / 2; ++j)
        {
            Vector2f start{-mWidth / 2.0f * mSquareSize, (float)j * mSquareSize};
            Vector2f end{mWidth / 2.0f * mSquareSize, (float)j * mSquareSize};
            Physics::Line l{start, end};
            gizmos->DrawLine(&l, color);
        }
    }
#endif
}
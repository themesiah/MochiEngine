#ifndef HDEF_ITEXTURE
#define HDEF_ITEXTURE

#include "../Types/Types.hpp"

namespace Mochi::Graphics
{
    class ITexture
    {
    public:
        virtual ~ITexture() {}
        virtual Vector2f GetSize() const = 0;
    };
}

#endif
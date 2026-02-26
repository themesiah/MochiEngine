#ifndef HDEF_ITEXTURE
#define HDEF_ITEXTURE

#include "../Types/Types.hpp"

namespace Mochi::Graphics
{
    /// @brief A texture interface to be implemented by renderer specific textures.
    class ITexture
    {
    public:
        virtual ~ITexture() {}
        virtual Vector2f GetSize() const = 0;
        virtual void SetAlpha(const uint8_t &alpha) = 0;
    };
}

#endif
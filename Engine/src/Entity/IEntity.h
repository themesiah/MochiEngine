#ifndef HDEF_IENTITY
#define HDEF_IENTITY

#include <cstdint>

namespace Mochi
{
    using EntityHandler = uint32_t;
    struct IEntity
    {
        virtual ~IEntity() {}
    };
}

#endif
#ifndef HDEF_IENTITY
#define HDEF_IENTITY

namespace Mochi
{

    using EntityHandler = uint32_t;
    struct IEntity
    {
        virtual ~IEntity() {}
    };
}

#endif
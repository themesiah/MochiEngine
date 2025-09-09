#ifndef HDEF_IKEYBOARDPROVIDER
#define HDEF_IKEYBOARDPROVIDER

#include <cstddef>

namespace Mochi::Input
{
    class IKeyboardProvider
    {
    public:
        virtual ~IKeyboardProvider() {}
        virtual const bool *GetState() const = 0;
        virtual size_t GetMaxKeys() const = 0;
    };
}

#endif
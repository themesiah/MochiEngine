#ifndef HDEF_SDLKEYBOARDPROVIDER
#define HDEF_SDLKEYBOARDPROVIDER

#include "IKeyboardProvider.h"

namespace Mochi::Input
{
    class SDLKeyboardProvider : public IKeyboardProvider
    {
    public:
        SDLKeyboardProvider();
        virtual ~SDLKeyboardProvider();
        virtual const bool *GetState() const override;
        virtual size_t GetMaxKeys() const override;
    };
}
#endif
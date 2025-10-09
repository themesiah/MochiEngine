#ifndef HDEF_DUMMYINPUTPROVIDERS
#define HDEF_DUMMYINPUTPROVIDERS

#include "IGamepadProvider.h"
#include "IMouseProvider.h"
#include "IKeyboardProvider.h"

namespace Mochi::Input
{
    class DummyKeyboardProvider : public IKeyboardProvider
    {
    public:
        virtual ~DummyKeyboardProvider() {}
        virtual const bool *GetState() const override { return {}; };
        virtual size_t GetMaxKeys() const override { return 0; };
    };
    class DummyMouseProvider : public IMouseProvider
    {
    public:
        virtual ~DummyMouseProvider() {}
        virtual std::array<bool, MouseButton::MouseButtonsCount> GetState(float *x, float *y) const override { return {false, false, false, false, false}; };
    };
    class DummyGamepadProvider : public IGamepadProvider
    {
    public:
        virtual ~DummyGamepadProvider() {}
        virtual GamepadData GetData(const unsigned int &player) const override { return {}; };
        virtual bool HasGamepad(const unsigned int &player) const override { return false; };
    };
}

#endif
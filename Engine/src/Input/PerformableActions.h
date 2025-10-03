#ifndef HDEF_ACTIONSPERFORMED
#define HDEF_ACTIONSPERFORMED

#include "IPerformableAction.h"

#include <nlohmann/json_fwd.hpp>

#include "InputCommons.h"

namespace Mochi::Input
{
    struct PerformableActionKeyboard : public IPerformableAction
    {
    private:
        int mKey;
        ActionTrigger mTrigger;
        float mValue;

    public:
        PerformableActionKeyboard(const nlohmann::json &json);
        virtual ~PerformableActionKeyboard();
        virtual bool IsPerformed(InputManager *input) const;
        virtual float GetValue(InputManager *input) const;
    };

    struct PerformableActionMouse : public IPerformableAction
    {
    private:
        MouseButton mButton;
        ActionTrigger mTrigger;
        float mValue;

    public:
        PerformableActionMouse(const nlohmann::json &json);
        virtual ~PerformableActionMouse();
        virtual bool IsPerformed(InputManager *input) const;
        virtual float GetValue(InputManager *input) const;
    };
    struct PerformableActionMouseAxis : public IPerformableAction
    {
    private:
        MouseAxis mAxis;
        float mScale;

    public:
        PerformableActionMouseAxis(const nlohmann::json &json);
        virtual ~PerformableActionMouseAxis();
        virtual bool IsPerformed(InputManager *input) const;
        virtual float GetValue(InputManager *input) const;
    };
    struct PerformableActionGamepad : public IPerformableAction
    {
    private:
        GamepadButton mButton;
        ActionTrigger mTrigger;
        float mValue;

    public:
        PerformableActionGamepad(const nlohmann::json &json);
        virtual ~PerformableActionGamepad();
        virtual bool IsPerformed(InputManager *input) const;
        virtual float GetValue(InputManager *input) const;
    };
    struct PerformableActionGamepadAxis : public IPerformableAction
    {
    private:
        GamepadAxis mAxis;
        float mScale;
        float mThreshold;

    public:
        PerformableActionGamepadAxis(const nlohmann::json &json);
        virtual ~PerformableActionGamepadAxis();
        virtual bool IsPerformed(InputManager *input) const;
        virtual float GetValue(InputManager *input) const;
    };
}

#endif
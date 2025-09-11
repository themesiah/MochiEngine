#ifndef HDEF_ACTIONSPERFORMED
#define HDEF_ACTIONSPERFORMED

#include "IPerformableAction.h"

#include <json.hpp>

#include "InputCommons.h"

using json = nlohmann::json;

namespace Mochi::Input
{
    struct PerformableActionKeyboard : public IPerformableAction
    {
    private:
        int mKey;
        ActionTrigger mTrigger;
        float mValue;

    public:
        PerformableActionKeyboard(const json &json);
        virtual ~PerformableActionKeyboard();
        virtual bool IsPerformed(std::shared_ptr<InputManager> input) const;
        virtual float GetValue(std::shared_ptr<InputManager> input) const;
    };

    struct PerformableActionMouse : public IPerformableAction
    {
    private:
        MouseButton mButton;
        ActionTrigger mTrigger;
        float mValue;

    public:
        PerformableActionMouse(const json &json);
        virtual ~PerformableActionMouse();
        virtual bool IsPerformed(std::shared_ptr<InputManager> input) const;
        virtual float GetValue(std::shared_ptr<InputManager> input) const;
    };
    struct PerformableActionMouseAxis : public IPerformableAction
    {
    private:
        MouseAxis mAxis;
        float mScale;

    public:
        PerformableActionMouseAxis(const json &json);
        virtual ~PerformableActionMouseAxis();
        virtual bool IsPerformed(std::shared_ptr<InputManager> input) const;
        virtual float GetValue(std::shared_ptr<InputManager> input) const;
    };
    struct PerformableActionGamepad : public IPerformableAction
    {
    private:
        GamepadButton mButton;
        ActionTrigger mTrigger;
        float mValue;

    public:
        PerformableActionGamepad(const json &json);
        virtual ~PerformableActionGamepad();
        virtual bool IsPerformed(std::shared_ptr<InputManager> input) const;
        virtual float GetValue(std::shared_ptr<InputManager> input) const;
    };
    struct PerformableActionGamepadAxis : public IPerformableAction
    {
    private:
        GamepadAxis mAxis;
        float mScale;

    public:
        PerformableActionGamepadAxis(const json &json);
        virtual ~PerformableActionGamepadAxis();
        virtual bool IsPerformed(std::shared_ptr<InputManager> input) const;
        virtual float GetValue(std::shared_ptr<InputManager> input) const;
    };
}

#endif
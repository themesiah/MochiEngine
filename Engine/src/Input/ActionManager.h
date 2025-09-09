#ifndef HDEF_ACTIONMANAGER
#define HDEF_ACTIONMANAGER

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <json.hpp>

using json = nlohmann::json;

namespace Mochi::Input
{

    enum ActionTrigger
    {
        Never = 0,
        Down = 1,
        Pressed = 2,
        Released = 3
    };

    struct KeyboardAction
    {
        int KeycodePositive;
        int KeycodeNegative;
        ActionTrigger Trigger;
    };
    struct MouseAction
    {
    };
    struct GamepadAction
    {
    };
    struct Action
    {
        KeyboardAction Keyboard;
        MouseAction Mouse;
        GamepadAction Gamepad;
    };
    void from_json(const json &j, Action &action);
    void from_json(const json &j, KeyboardAction &action);
    void from_json(const json &j, MouseAction &action);
    void from_json(const json &j, GamepadAction &action);

    class InputManager;
    class ActionManager
    {
    private:
        std::shared_ptr<InputManager> mInputManager;
        std::map<std::string, Action> mActions;

    public:
        ActionManager(std::shared_ptr<InputManager> inputManager);
        std::shared_ptr<InputManager> GetInputManager() const { return mInputManager; }
        bool LoadActions(std::vector<char>);
        bool LoadActionsFromFile(const std::string &path);
        bool HasAction(const std::string &) const;
        void Update(const float &delta);
        bool Performed(const std::string &actionName) const;
        float Value(const std::string &actionName) const;
    };
}

#endif
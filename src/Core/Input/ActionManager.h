#ifndef HDEF_ACTIONMANAGER
#define HDEF_ACTIONMANAGER

#include <string>
#include <memory>
#include <map>
#include <vector>

class InputManager;
class ActionManager
{
private:
    enum ActionTrigger
    {
        Never = 0,
        Down = 1,
        Pressed = 2,
        Released = 3
    };
    struct Action
    {
        int KeycodePositive;
        int KeycodeNegative;
        ActionTrigger Trigger;
    };
    std::shared_ptr<InputManager> mInputManager;
    std::map<std::string, Action> mActions;

public:
    ActionManager(std::shared_ptr<InputManager> inputManager);
    bool LoadActions(std::vector<char>);
    bool LoadActionsFromFile(const std::string &path);
    bool HasAction(const std::string &) const;
    void Update(const float &delta, const bool *keyboardState);
    bool Performed(const std::string &actionName) const;
    float Value(const std::string &actionName) const;
};

#endif
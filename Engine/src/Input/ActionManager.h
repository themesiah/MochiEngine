#ifndef HDEF_ACTIONMANAGER
#define HDEF_ACTIONMANAGER

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <json.hpp>

#include "InputCommons.h"
#include "IPerformableAction.h"
#include "../Types/Types.hpp"

using json = nlohmann::json;

namespace Mochi::Input
{
    struct Action
    {
        std::vector<std::shared_ptr<IPerformableAction>> PerformableActions;
    };
    void from_json(const json &j, Action &action);

    class InputManager;
    class ActionManager
    {
    private:
        std::shared_ptr<InputManager> mInputManager;
        std::map<std::string, Action> mActions;

    public:
        ActionManager(std::shared_ptr<InputManager> inputManager);
        bool LoadActions(std::vector<char>);
        bool LoadActionsFromFile(const std::string &path);
        bool HasAction(const std::string &) const;
        void Update(const float &delta);
        bool Performed(const std::string &actionName) const;
        float Value(const std::string &actionName) const;
        Vector2f CompoundValue(const std::string &action1, const std::string &action2) const;
    };
}

#endif
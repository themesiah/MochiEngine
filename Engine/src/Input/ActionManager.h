#ifndef HDEF_ACTIONMANAGER
#define HDEF_ACTIONMANAGER

#include "IActionManager.h"

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <nlohmann/json_fwd.hpp>

#include "InputCommons.h"
#include "IPerformableAction.h"
#include "../Types/Types.hpp"

namespace Mochi::Input
{
    struct Action
    {
        std::vector<std::shared_ptr<IPerformableAction>> PerformableActions;
    };
    void from_json(const nlohmann::json &j, Action &action);

    class InputManager;
    class ActionManager : public IActionManager
    {
    private:
        std::unique_ptr<InputManager> mInputManager;
        std::map<std::string, Action> mActions;

    public:
        ActionManager(InputManager *inputManager);
        virtual ~ActionManager();
        bool LoadActions(std::vector<char>) override;
        bool LoadActionsFromFile(const std::string &path) override;
        bool HasAction(const std::string &) const override;
        void Update(const float &delta) override;
        bool Performed(const std::string &actionName) const override;
        float Value(const std::string &actionName) const override;
        Vector2f CompoundValue(const std::string &action1, const std::string &action2) const override;
    };
}

#endif
#ifndef HDEF_VIRTUALACTIONMANAGER
#define HDEF_VIRTUALACTIONMANAGER

#include "IActionManager.h"

#include <unordered_map>
#include <string>

namespace Mochi::Input
{
    /**
     * @brief An implementation of IActionManager that receives mock action states and allows to poll them when needed.
     * This is useful for testing and can be a base for emulating player actions on replays or similar features.
     */
    class VirtualActionManager : public IActionManager
    {
    private:
        std::unordered_map<std::string, float> mActionsState;

    public:
        VirtualActionManager();
        virtual ~VirtualActionManager();
        virtual bool LoadActions(std::vector<char> buffer) override;
        virtual bool LoadActionsFromFile(const std::string &path) override;
        virtual bool HasAction(const std::string &) const override;
        virtual void Update(const float &dt) override;
        virtual bool Performed(const std::string &actionName) const override;
        virtual float Value(const std::string &actionName) const override;
        virtual Vector2f CompoundValue(const std::string &action1, const std::string &action2) const override;

        /// @brief Sets an action on the action map to be retrieved later. This action is not changed unless SetActionState is called again with a different value.
        /// @param action The name of the action to change.
        /// @param state The value of the action. Note that for checking if an action has been performed the value has to be different from 0.
        void SetActionState(const std::string &action, const float &state);
    };
}

#endif
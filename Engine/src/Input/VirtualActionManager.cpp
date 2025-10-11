#include "VirtualActionManager.h"

#include <string>

#include "../Utils/MathUtils.h"

namespace Mochi::Input
{
    VirtualActionManager::VirtualActionManager() : mActionsState()
    {
    }

    VirtualActionManager::~VirtualActionManager()
    {
    }

    bool VirtualActionManager::LoadActions(std::vector<char> buffer)
    {
        return true;
    }

    bool VirtualActionManager::LoadActionsFromFile(const std::string &path)
    {
        return true;
    }

    bool VirtualActionManager::HasAction(const std::string &) const
    {
        return true;
    }

    void VirtualActionManager::Update(const float &dt)
    {
    }

    bool VirtualActionManager::Performed(const std::string &actionName) const
    {
        return !Math::Approx(Value(actionName), 0.0f);
    }

    float VirtualActionManager::Value(const std::string &actionName) const
    {
        auto it = mActionsState.find(actionName);
        if (it == mActionsState.end())
            return 0.0f;

        return it->second;
    }

    Vector2f VirtualActionManager::CompoundValue(const std::string &action1, const std::string &action2) const
    {
        return {Value(action1), Value(action2)};
    }

    void VirtualActionManager::SetActionState(const std::string &action, const float &state)
    {
        mActionsState[action] = state;
    }

}
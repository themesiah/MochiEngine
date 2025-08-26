#include "ActionManager.h"
#include "InputManager.h"

#include <fstream>
#include <json.hpp>
#include <iostream>

using json = nlohmann::json;

ActionManager::ActionManager(std::shared_ptr<InputManager> inputManager) : mInputManager(inputManager)
{
}

bool ActionManager::LoadActions(const std::string &actionsFile)
{
    std::ifstream f(actionsFile);
    json data = json::parse(f);
    mActions.clear();

    for (unsigned int i = 0; i < data["Actions"].size(); ++i)
    {
        auto entry = data["Actions"][i];
        std::string name = entry["Name"];
        int trigger = entry["Trigger"];
        int positive = entry["KeycodePositive"];
        int negative = entry["KeycodeNegative"];
        Action action;
        action.Trigger = (ActionTrigger)trigger;
        action.KeycodePositive = positive;
        action.KeycodeNegative = negative;
        mActions[name] = action;
    }
    return true;
}

bool ActionManager::HasAction(const std::string &actionName) const
{
    return mActions.find(actionName) != mActions.end();
}

void ActionManager::Update(const float &delta, const bool *keyboardState)
{
    mInputManager->Update(keyboardState);
}

bool ActionManager::Performed(const std::string &actionName) const
{
    if (!HasAction(actionName))
    {
        return false;
    }
    return Value(actionName) != 0.0f;
}

float ActionManager::Value(const std::string &actionName) const
{
    if (!HasAction(actionName))
    {
        return 0.0f;
    }
    Action action = mActions.find(actionName)->second;
    float value = 0.0f;
    switch (action.Trigger)
    {
    case ActionTrigger::Down:
        if (mInputManager->IsDown(action.KeycodePositive))
            value += 1.0f;
        if (mInputManager->IsDown(action.KeycodeNegative))
            value -= 1.0f;
        break;
    case ActionTrigger::Pressed:
        if (mInputManager->WasPressed(action.KeycodePositive))
            value += 1.0f;
        if (mInputManager->WasPressed(action.KeycodeNegative))
            value -= 1.0f;
        break;
    case ActionTrigger::Released:
        if (mInputManager->WasReleased(action.KeycodePositive))
            value += 1.0f;
        if (mInputManager->WasReleased(action.KeycodeNegative))
            value -= 1.0f;
        break;
    default:
        value = 0.0f;
    }
    return value;
}

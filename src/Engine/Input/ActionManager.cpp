#include "ActionManager.h"
#include "InputManager.h"

#include <fstream>
#include <json.hpp>
#include <iostream>

#include "../Assert.h"

using json = nlohmann::json;

ActionManager::ActionManager(std::shared_ptr<InputManager> inputManager) : mInputManager(inputManager)
{
}

bool ActionManager::LoadActions(const std::string &actionsFile)
{
    std::ifstream f(actionsFile);
    ASSERT(std::format("Can't open actions file on {}", actionsFile), !f.fail());
    json data = json::parse(f);
    f.close();
    mActions.clear();
    ASSERT("Actions data can't be empty", !data["Actions"].empty());
    for (unsigned int i = 0; i < data["Actions"].size(); ++i)
    {
        Action action;
        action.KeycodePositive = -1;
        action.KeycodeNegative = -1;
        action.Trigger = ActionTrigger::Never;

        json entry = data["Actions"][i];
        ASSERT("All actions need a string as a Name", entry.contains("Name") && entry["Name"].is_string());
        std::string name = entry["Name"];

        ASSERT("All actions need a trigger", entry.contains("Trigger") && entry["Trigger"].is_number_integer());
        action.Trigger = (ActionTrigger)entry["Trigger"];

        if (entry.contains("KeycodePositive"))
        {
            ASSERT("KeycodePositive must be an integer", entry["KeycodePositive"].is_number_integer());
            action.KeycodePositive = entry["KeycodePositive"];
        }
        if (entry.contains("KeycodeNegative"))
        {
            ASSERT("KeycodeNegative must be an integer", entry["KeycodeNegative"].is_number_integer());
            action.KeycodeNegative = entry["KeycodeNegative"];
        }
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
        if (action.KeycodePositive >= 0 && mInputManager->IsDown(action.KeycodePositive))
            value += 1.0f;
        if (action.KeycodeNegative >= 0 && mInputManager->IsDown(action.KeycodeNegative))
            value -= 1.0f;
        break;
    case ActionTrigger::Pressed:
        if (action.KeycodePositive >= 0 && mInputManager->WasPressed(action.KeycodePositive))
            value += 1.0f;
        if (action.KeycodeNegative >= 0 && mInputManager->WasPressed(action.KeycodeNegative))
            value -= 1.0f;
        break;
    case ActionTrigger::Released:
        if (action.KeycodePositive >= 0 && mInputManager->WasReleased(action.KeycodePositive))
            value += 1.0f;
        if (action.KeycodeNegative >= 0 && mInputManager->WasReleased(action.KeycodeNegative))
            value -= 1.0f;
        break;
    default:
        value = 0.0f;
    }
    return value;
}

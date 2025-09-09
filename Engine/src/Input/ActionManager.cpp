#include "ActionManager.h"
#include "InputManager.h"

#include <fstream>
#include <iostream>
#include <json.hpp>

#include "../Utils/Assert.h"
#include "../Utils/Logger.h"
#include "../Exception.hpp"

using json = nlohmann::json;

namespace Mochi::Input
{
    ActionManager::ActionManager(std::shared_ptr<InputManager> inputManager) : mInputManager(inputManager)
    {
    }

    bool ActionManager::LoadActions(std::vector<char> jsonContent)
    {
        try
        {
            json data = json::parse(jsonContent);
            mActions.clear();
            ASSERT("Actions data can't be empty", !data["Actions"].empty());
            for (unsigned int i = 0; i < data["Actions"].size(); ++i)
            {
                json entry = data["Actions"][i];
                ASSERT("All actions need a string as a Name", entry.contains("Name") && entry["Name"].is_string());
                std::string name = entry["Name"];

                auto p = entry.template get<Action>();
                mActions[name] = p;
            }
            return true;
        }
        catch (const std::exception &e)
        {
            throw EngineError(e.what());
        }
    }

    void from_json(const json &entry, Action &action)
    {
        json keyboard = entry["Keyboard"];
        auto k = keyboard.template get<KeyboardAction>();
        action.Keyboard = k;

        json gamepad = entry["Gamepad"];
        auto g = gamepad.template get<GamepadAction>();
        action.Gamepad = g;

        json mouse = entry["Mouse"];
        auto m = mouse.template get<MouseAction>();
        action.Mouse = m;
    }

    void from_json(const json &keyboard, KeyboardAction &action)
    {
        action.KeycodeNegative = -1;
        action.KeycodePositive = -1;
        action.Trigger = ActionTrigger::Never;

        ASSERT("All actions need a trigger", keyboard.contains("Trigger") && keyboard["Trigger"].is_number_integer());
        action.Trigger = (ActionTrigger)keyboard["Trigger"];

        if (keyboard.contains("KeycodePositive"))
        {
            ASSERT("KeycodePositive must be an integer", keyboard["KeycodePositive"].is_number_integer());
            action.KeycodePositive = keyboard["KeycodePositive"];
        }
        if (keyboard.contains("KeycodeNegative"))
        {
            ASSERT("KeycodeNegative must be an integer", keyboard["KeycodeNegative"].is_number_integer());
            action.KeycodeNegative = keyboard["KeycodeNegative"];
        }
    }

    void from_json(const json &j, GamepadAction &action)
    {
    }

    void from_json(const json &j, MouseAction &action)
    {
    }

    bool ActionManager::LoadActionsFromFile(const std::string &actionsFile)
    {
        std::ifstream f(actionsFile);
        ASSERT(std::format("Can't open actions file on {}", actionsFile), !f.fail());
        if (f.fail())
            throw ResourceNotFoundError(actionsFile);
        std::vector<char> data((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        f.close();
        return LoadActions(data);
    }

    bool ActionManager::HasAction(const std::string &actionName) const
    {
        return mActions.find(actionName) != mActions.end();
    }

    void ActionManager::Update(const float &delta)
    {
        mInputManager->Update();
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
        switch (action.Keyboard.Trigger)
        {
        case ActionTrigger::Down:
            if (action.Keyboard.KeycodePositive >= 0 && mInputManager->IsDown(action.Keyboard.KeycodePositive))
                value += 1.0f;
            if (action.Keyboard.KeycodeNegative >= 0 && mInputManager->IsDown(action.Keyboard.KeycodeNegative))
                value -= 1.0f;
            break;
        case ActionTrigger::Pressed:
            if (action.Keyboard.KeycodePositive >= 0 && mInputManager->WasPressed(action.Keyboard.KeycodePositive))
                value += 1.0f;
            if (action.Keyboard.KeycodeNegative >= 0 && mInputManager->WasPressed(action.Keyboard.KeycodeNegative))
                value -= 1.0f;
            break;
        case ActionTrigger::Released:
            if (action.Keyboard.KeycodePositive >= 0 && mInputManager->WasReleased(action.Keyboard.KeycodePositive))
                value += 1.0f;
            if (action.Keyboard.KeycodeNegative >= 0 && mInputManager->WasReleased(action.Keyboard.KeycodeNegative))
                value -= 1.0f;
            break;
        default:
            value = 0.0f;
        }
        return value;
    }
}

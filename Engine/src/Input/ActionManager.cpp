#include "ActionManager.h"
#include "InputManager.h"

#include <fstream>
#include <iostream>
#include <json.hpp>

#include "../Utils/Assert.h"
#include "../Utils/Logger.h"
#include "../Utils/MathUtils.h"
#include "../Exception.hpp"

#include "PerformableActions.h"
#include "IPerformableAction.h"

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
            auto actions = data.at("Actions");
            ASSERT("Actions data can't be empty", !actions.empty());
            for (size_t i = 0; i < actions.size(); ++i)
            {
                try
                {
                    json entry = actions[i];
                    ASSERT("All actions need a string as a Name", entry.contains("Name") && entry.at("Name").is_string());
                    std::string name = entry.at("Name");

                    auto p = entry.template get<Action>();
                    mActions[name] = p;
                }
                catch (const MalformedInputAction &e)
                {
                    LOG_WARNING(e.what());
                }
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
        if (!entry.contains("Cases"))
        {
            throw MalformedInputAction(std::source_location::current());
        }

        auto cases = entry.at("Cases");
        for (size_t i = 0; i < cases.size(); ++i)
        {
            auto actionCase = cases[i];
            if (!actionCase.contains("Type") || !actionCase.at("Type").is_string())
            {
                continue;
            }
            std::string type = actionCase.at("Type");
            if (type == "Keyboard")
            {
                action.PerformableActions.push_back(std::make_shared<PerformableActionKeyboard>(actionCase));
            }
            else if (type == "Mouse")
            {
                action.PerformableActions.push_back(std::make_shared<PerformableActionMouse>(actionCase));
            }
            else if (type == "MouseAxis")
            {
                action.PerformableActions.push_back(std::make_shared<PerformableActionMouseAxis>(actionCase));
            }
            else if (type == "Gamepad")
            {
                action.PerformableActions.push_back(std::make_shared<PerformableActionGamepad>(actionCase));
            }
            else if (type == "GamepadAxis")
            {
                action.PerformableActions.push_back(std::make_shared<PerformableActionGamepadAxis>(actionCase));
            }
            else
            {
                LOG_WARNING(std::format("Performable action of type {} does not exist. Did you do a typo?", type));
                continue;
            }
        }

        if (action.PerformableActions.size() == 0)
        {
            throw MalformedInputAction(std::source_location::current());
        }
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
        Action action = mActions.at(actionName);
        for (auto performableAction : action.PerformableActions)
        {
            if (performableAction->IsPerformed(mInputManager))
                return true;
        }
        return false;
    }

    float ActionManager::Value(const std::string &actionName) const
    {
        if (!HasAction(actionName))
        {
            return 0.0f;
        }
        Action action = mActions.at(actionName);
        float value = 0.0f;
        for (auto performableAction : action.PerformableActions)
        {
            value += performableAction->GetValue(mInputManager);
        }
        if (Math::Approx(value, 0.0f))
            return 0.0f;

        return value;
    }

    Vector2f ActionManager::CompoundValue(const std::string &action1, const std::string &action2) const
    {
        float x = Value(action1);
        float y = Value(action2);
        return {x, y};
    }
}

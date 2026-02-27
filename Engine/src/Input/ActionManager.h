#ifndef HDEF_ACTIONMANAGER
#define HDEF_ACTIONMANAGER

#include "IActionManager.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <nlohmann/json_fwd.hpp>

#include "InputCommons.h"
#include "IPerformableAction.h"
#include "../Types/Types.hpp"

namespace Mochi::Input
{
    /// @brief Set of performable actions that will be used to compute an "or" operation, making an action be considered performed
    /// if any of its performable actions are being performed.
    struct Action
    {
        std::vector<std::shared_ptr<IPerformableAction>> PerformableActions;
    };
    void from_json(const nlohmann::json &j, Action &action);

    class InputManager;
    /**
     * @brief Implementation of IActionManager meant to be used on any context where the user has control over the game.
     * Expected flow for this class is creating it, loading the actions config file, updating it each frame and checking the state
     * of the actions at any time.
     *
     * NOTE: Actions data is expected as a JSON. Check [this example](./Resources/Actions.json) for a first example of the format.
     *
     * Actions are a set of PerformableAction, which defines when an action is considered as performed and how it gets its value.
     *
     * Related and important classes or structures:
     *
     * - Action
     *
     * - IPerformableAction, PerformableActionKeyboard, PerformableActionMouse, PerformableActionMouseAxis, PerformableActionGamepad, PerformableActionGamepadAxis
     *
     * - InputManager
     *
     * - IGamepadProvider, IKeyboardProvider, IMouseProvider
     *
     * Ownership:
     *
     * - Owns the input manager.
     *
     * - Owns a set of actions, loaded from the data JSON.
     *
     * Guarantees:
     *
     * - An updated state of each action each frame, accessible via polling (Performed, Value, CompoundValue)
     *
     *
     * Non responsible:
     *
     * - Dispatching events when an action is performed.
     *
     * Thread safety:
     *
     * - Polling data is thread safe, as it doesn't modify the class.
     *
     * - Update is not thread safe.
     */
    class ActionManager : public IActionManager
    {
    private:
        std::unique_ptr<InputManager> mInputManager;
        std::unordered_map<std::string, Action> mActions;

    public:
        /// @param inputManager A unique pointer to the input manager. Reference will be moved by this class to take ownership of the unique pointer.
        ActionManager(std::unique_ptr<InputManager> inputManager);
        virtual ~ActionManager();

        /// @brief Loads a list of actions from a JSON string data.
        /// @throws MalformedInputAction if JSON has not the correct format.
        /// @param data The string JSON with the actions data.
        /// @return True if loaded correctly.
        bool LoadActions(std::vector<char> data) override;
        /// @brief Loads a file from the system (without using the PackCatalog) and calls LoadActions with its content.
        /// @param path The path to the file.
        /// @return True if loaded correctly.
        bool LoadActionsFromFile(const std::string &path) override;
        /// @brief Checks if there is a action with the provided name in the list.
        /// @param actionName The name of the action to check.
        /// @return True if the action exists, false otherwise.
        bool HasAction(const std::string &actionName) const override;
        /// @brief Updates the internal input manager. Check InputManager.
        /// @param delta The delta time.
        void Update(const float &delta) override;
        /// @brief Checks if an action has been performed this frame.
        /// @param actionName The name of the action.
        /// @return True if the action has been performed this frame. An analog action is true if its value is different from 0.
        bool Performed(const std::string &actionName) const override;
        /// @brief Gets the analog value of an action.
        /// @param actionName The name of the action.
        /// @return The analog value of the action.
        float Value(const std::string &actionName) const override;
        /// @brief Utility method to get two values at the same time. Useful to get values on vertical and horizontal axis at the same time, for example.
        /// @param action1 First action to check.
        /// @param action2 Second action to check.
        /// @return Vector with the two resulting values.
        Vector2f CompoundValue(const std::string &action1, const std::string &action2) const override;
    };
}

#endif
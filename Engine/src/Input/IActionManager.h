#ifndef HDEF_IACTIONMANAGER
#define HDEF_IACTIONMANAGER

#include <string>
#include <vector>

#include "../Types/Types.hpp"

namespace Mochi::Input
{
    /**
     * @brief IActionManager is an interface with the purpose of giving the ability to identify which actions
     * are being performed by the player.
     * Do not mistake this by an input manager that returns true or false if some key is pressed. An action manager
     * should identify is a button or key has been pressed this frame, or released, or the relative value of an analog control
     * like a joystick. The responsibility of those features comes with the implementation.
     *
     * Actually, a single ActionManager implementation should be enough for any game, but this interface allows us to
     * implement also VirtualActionManager, which is useful for tests. Check those two classes for more information
     * on the implemented methods.
     */
    class IActionManager
    {
    public:
        virtual ~IActionManager() {}
        virtual bool LoadActions(std::vector<char> buffer) = 0;
        virtual bool LoadActionsFromFile(const std::string &path) = 0;
        virtual bool HasAction(const std::string &) const = 0;
        virtual void Update(const float &dt) = 0;
        virtual bool Performed(const std::string &actionName) const = 0;
        virtual float Value(const std::string &actionName) const = 0;
        virtual Vector2f CompoundValue(const std::string &action1, const std::string &action2) const = 0;
    };
}

#endif
#ifndef HDEF_IACTIONMANAGER
#define HDEF_IACTIONMANAGER

#include <string>
#include <vector>

#include "../Types/Types.hpp"

namespace Mochi::Input
{
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
#ifndef HDEF_VIRTUALACTIONMANAGER
#define HDEF_VIRTUALACTIONMANAGER

#include "IActionManager.h"

#include <unordered_map>
#include <string>

namespace Mochi::Input
{
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

        void SetActionState(const std::string &action, const float &state);
    };
}

#endif
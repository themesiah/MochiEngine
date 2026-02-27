#ifndef HDEF_IACTIONPERFORMED
#define HDEF_IACTIONPERFORMED

#include <memory>

namespace Mochi::Input
{
    class InputManager;
    /// @brief An interface defining if an action is being performed and what its value is for a single trigger.
    struct IPerformableAction
    {
    public:
        virtual ~IPerformableAction() {}
        virtual bool IsPerformed(InputManager *input) const = 0;
        virtual float GetValue(InputManager *input) const = 0;
    };
}

#endif
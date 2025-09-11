#ifndef HDEF_IACTIONPERFORMED
#define HDEF_IACTIONPERFORMED

#include <memory>

namespace Mochi::Input
{
    class InputManager;
    struct IPerformableAction
    {
    public:
        virtual ~IPerformableAction() {}
        virtual bool IsPerformed(std::shared_ptr<InputManager> input) const = 0;
        virtual float GetValue(std::shared_ptr<InputManager> input) const = 0;
    };
}

#endif
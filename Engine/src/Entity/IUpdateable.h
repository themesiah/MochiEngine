#ifndef HDEF_IUPDATEABLE
#define HDEF_IUPDATEABLE

namespace Mochi
{
    namespace Input
    {
        class ActionManager;
    }
    struct IUpdateable
    {
        virtual ~IUpdateable() {}
        virtual void Update(const float &dt, std::shared_ptr<Input::ActionManager>) = 0;
    };
}

#endif
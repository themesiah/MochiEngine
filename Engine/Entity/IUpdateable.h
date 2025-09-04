#ifndef HDEF_IUPDATEABLE
#define HDEF_IUPDATEABLE

class ActionManager;
struct IUpdateable
{
    virtual ~IUpdateable() {}
    virtual void Update(const float &dt, std::shared_ptr<ActionManager>) = 0;
};

#endif
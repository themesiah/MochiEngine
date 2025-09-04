#ifndef HDEF_IUPDATEABLE
#define HDEF_IUPDATEABLE

struct IUpdateable
{
    virtual ~IUpdateable() {}
    virtual void Update(const float &dt) = 0;
};

#endif
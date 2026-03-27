#ifndef HDEF_LEFTRIGHTCOMPONENT
#define HDEF_LEFTRIGHTCOMPONENT

namespace Mochi::Platformer
{
    struct LeftRightComponent
    {
        float Direction = 1.0f;
        uint32_t LayerMaskCheck = 2;
        float RaycastLength = 0.5f;
    };
}

#endif
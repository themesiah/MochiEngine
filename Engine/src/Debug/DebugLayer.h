#ifndef HDEF_DEBUGLAYER
#define HDEF_DEBUGLAYER

#include "../Layer.h"

namespace Mochi::Graphics
{
    class Renderer;
    class GUI;
}
namespace Mochi
{
    /**
     * @brief The debug layer is a layer automatically injected in the Engine if the game is in debug mode. It provides a text indicating that it is a development build,
     * an FPS counter and two perpendicular lines crossing at the center of the screen.
     */
    class DebugLayer : public Layer
    {
    public:
        DebugLayer();
        ~DebugLayer();
        virtual void InitLayer() override;
        virtual bool Update(const float &dt) override;
        virtual void Render() const override;
        virtual void GUI() override;
        virtual void Debug() const override;
    };
}

#endif
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
    class DebugLayer : public Layer
    {
    private:
        Graphics::Renderer *mRenderer;
        Graphics::GUI *mGUI;

    public:
        DebugLayer(Graphics::Renderer *renderer, Graphics::GUI *gui);
        ~DebugLayer();
        virtual bool Update(const float &dt) override;
        virtual void Render() const override;
        virtual void GUI() const override;
        virtual void Debug() const override;
    };
}

#endif
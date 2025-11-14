#ifndef HDEF_SCRIPTINGLAYER
#define HDEF_SCRIPTINGLAYER

#include <memory>
#include <vector>

#include "../Layer.h"

namespace Mochi::Graphics
{
    class SpriteBase;
    class AbstractTextureFactory;
    class IAnimationFactory;
}
namespace Mochi::Scripting
{
    class ScriptingLayer : public Layer
    {
    private:
        std::unique_ptr<Graphics::AbstractTextureFactory> mTextureFactory;
        std::unique_ptr<Graphics::IAnimationFactory> mAnimationFactory;
        std::vector<std::shared_ptr<Graphics::SpriteBase>> mSprites;
        std::vector<std::shared_ptr<Graphics::SpriteBase>> mSpritesToDestroy;
        void BindFactoryMethods();

    public:
        ScriptingLayer();
        virtual ~ScriptingLayer();
        virtual void InitLayer() override;
        virtual bool Update(const float &dt) override;
        virtual void Render() const override;
        virtual void GUI() const override;
#if DEBUG
        virtual void Debug() const override;
#endif
    };
}

#endif
#ifndef HDEF_PLAYER
#define HDEF_PLAYER

#include <memory>
#include "Entity/IEntity.h"
#include "Entity/IAnimatable.h"
#include "Entity/IRenderable.h"
#include "Entity/IUpdateable.h"
#include "Graphics/RenderCommand.h"
#include "Types/Types.hpp"

namespace Mochi::Graphics
{
    class AnimatedSprite;
    class AnimationFactory;
    class TextureFactory;
}
namespace Mochi::Input
{
    class ActionManager;
}
class Player : public Mochi::IEntity, public Mochi::IAnimatable, public Mochi::IRenderable, public Mochi::IUpdateable
{
private:
    std::unique_ptr<Mochi::Graphics::AnimatedSprite> mAnim;
    Mochi::Vector2f mPosition;
    float mSpeed;

public:
    Player(std::shared_ptr<Mochi::Graphics::AnimationFactory>, std::shared_ptr<Mochi::Graphics::TextureFactory>);
    ~Player();
    virtual void UpdateAnimation(const float &dt);
    virtual std::vector<Mochi::Graphics::RenderCommand> GetRenderData() const;
    virtual void Update(const float &dt, std::shared_ptr<Mochi::Input::ActionManager>);
};

#endif
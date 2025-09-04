#ifndef HDEF_WORLD
#define HDEF_WORLD

#include <memory>
#include <vector>
#include <unordered_map>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL.h>

typedef uint32_t EntityHandler;

class Sprite;
class AnimatedSprite;
class FMODWrapper;
class ActionManager;
class PackCatalog;
class TextureFactory;
class AnimationFactory;
class Camera;
class Renderer;
struct TTF_Font;
struct TTF_TextEngine;
struct TTF_Text;
struct IEntity;
struct IUpdateable;
struct IRenderable;
struct IAnimatable;
class Engine
{
private:
    int mTargetFPS;
    int64_t mNsPerFrame;
    float mLastDeltaTime;
    void Render() const;

    EntityHandler mLastEntityHandler;
    std::vector<EntityHandler> mFreeEntityHandlers;
    std::unordered_map<EntityHandler, std::shared_ptr<IEntity>> mEntities;
    std::vector<std::shared_ptr<IUpdateable>> mUpdateables;
    std::vector<std::shared_ptr<IRenderable>> mRenderables;
    std::vector<std::shared_ptr<IAnimatable>> mAnimatables;

    // TEMP
    TTF_Font *mFont;
    TTF_TextEngine *mTextEngine;
    TTF_Text *mText;
    // END TEMP

protected:
    std::shared_ptr<Renderer> mRenderer;
    std::shared_ptr<FMODWrapper> mFmod;
    std::shared_ptr<ActionManager> mActionManager;
    std::shared_ptr<PackCatalog> mCatalog;
    std::shared_ptr<TextureFactory> mTextureFactory;
    std::shared_ptr<AnimationFactory> mAnimationFactory;
    std::shared_ptr<Camera> mCamera;
    virtual bool OnUpdate(const float &dt) = 0;
    EntityHandler AddEntity(std::shared_ptr<IEntity>);
    bool RemoveEntity(EntityHandler);

public:
    Engine();
    bool Update();
    virtual ~Engine();
};

#endif
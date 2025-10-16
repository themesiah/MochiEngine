#include "ScriptingLayer.h"

#include <sol/sol.hpp>

#include "../Graphics/SpriteBase.h"
#include "../Graphics/Spritesheet.h"
#include "../Graphics/AnimatedSprite.h"
#include "../Graphics/OneshotAnimation.h"
#include "../Graphics/AsepriteAnimationFactory.h"
#include "../Graphics/IRenderer.h"
#include "../Graphics/AbstractTextureFactory.h"
#include "LuaBindings.h"
#include "ScriptingManager.h"

#include "../Utils/Logger.h"

namespace Mochi::Scripting
{

    ScriptingLayer::ScriptingLayer() : mSprites({}), mSpritesToDestroy({})
    {
        mTextureFactory = mRenderer->CreateTextureFactory(mCatalog);
        mAnimationFactory = std::make_unique<Graphics::AsepriteAnimationFactory>(mCatalog);

        Bindings::BindLogic(mScripting, mActionManager);
        Bindings::BindGraphics(mScripting, mCamera);
        Bindings::BindAudio(mScripting, mAudioManager);
        BindFactoryMethods();
        LOG_OK("LUA Factory methods and classes Binded");
    }

    ScriptingLayer::~ScriptingLayer()
    {
    }

    void ScriptingLayer::BindFactoryMethods()
    {
        mScripting->State.set_function("CreateSprite", [this](const std::string &resourcePath)
                                       { std::shared_ptr<Graphics::SpriteBase> sprite = std::make_shared<Graphics::SpriteBase>(mTextureFactory.get(), resourcePath);
                                        mSprites.push_back(sprite);
                                        return sprite; });
        mScripting->State.set_function("CreateSpritesheet", [this](const std::string &resourcePath, const int &startingFrame)
                                       { std::shared_ptr<Graphics::Spritesheet> sprite = std::make_shared<Graphics::Spritesheet>(mAnimationFactory.get(), mTextureFactory.get(), resourcePath, startingFrame);
                                        mSprites.push_back(sprite);
                                        return sprite; });
        mScripting->State.set_function("CreateAnimatedSprite", [this](const std::string &resourcePath, const std::string &mainAnimation)
                                       { std::shared_ptr<Graphics::AnimatedSprite> sprite = std::make_shared<Graphics::AnimatedSprite>(mAnimationFactory.get(), mTextureFactory.get(), resourcePath, mainAnimation);
                                        mSprites.push_back(sprite);
                                        return sprite; });
        mScripting->State.set_function("CreateOneshotAnimation", [this](const std::string &resourcePath, const std::string &mainAnimation)
                                       { std::shared_ptr<Graphics::OneshotAnimation> sprite = std::make_shared<Graphics::OneshotAnimation>(mAnimationFactory.get(), mTextureFactory.get(), resourcePath, mainAnimation);
                                        mSprites.push_back(sprite);
                                        return sprite; });

        mScripting->State.set_function("DeleteSprite", [this](std::shared_ptr<Graphics::SpriteBase> deletableElement)
                                       { mSpritesToDestroy.push_back(deletableElement); });
    }

    bool ScriptingLayer::Update(const float &dt)
    {
        for (auto &sprite : mSprites)
        {
            sprite->Update(dt);
        }

        for (const auto &outSprite : mSpritesToDestroy)
        {
            mSprites.erase(
                std::remove_if(mSprites.begin(), mSprites.end(),
                               [&](const std::shared_ptr<Graphics::SpriteBase> &sprite)
                               { return sprite.get() == outSprite.get(); }),
                mSprites.end());
        }
        mSpritesToDestroy.clear();
        return true;
    }

    void ScriptingLayer::Render() const
    {
        auto &engine = Engine::Get();
        for (auto &sprite : mSprites)
        {
            engine.AddRenderCommand(sprite->GetRenderData());
        }
    }

    void ScriptingLayer::GUI() const
    {
    }

#if DEBUG
    void ScriptingLayer::Debug() const
    {
    }

#endif
}
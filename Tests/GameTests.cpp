// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <memory>

#include "Engine.h"
#include "Graphics/DummyRendering.h"
#include "Input/VirtualActionManager.h"
#include "Audio/DummyAudioManager.h"
#include "Layer.h"
#include "Graphics/AsepriteAnimationFactory.h"

#include "Enemies/Enemy.h"
#include "Player.h"
#include "Bullets/PlayerBulletPool.h"

#include <iostream>

using namespace Mochi;

class TestLayer : public Layer
{
public:
    TestLayer() : mTimer(1.0f)
    {
        mTextureFactory = mRenderer->CreateTextureFactory(mCatalog);
        mAnimationFactory = std::make_unique<Graphics::AsepriteAnimationFactory>(mCatalog);
        mEnemy = std::make_unique<Shooter::Enemy>(mEventBus, mTextureFactory.get());
        mPlayer = std::make_unique<Shooter::Player>(mAnimationFactory.get(), mTextureFactory.get(), mCamera, mActionManager);
        mEnemy->SetScale(2.0f);
        mPlayer->SetScale(2.0f);
    }

    virtual ~TestLayer() {}
    virtual bool Update(const float &dt)
    {
        mPlayer->Update(dt);
        auto playerBulletPool = mPlayer->GetBulletPool();
        auto enemyShape = mEnemy->GetCollider();
        std::vector<int> collisions = playerBulletPool->CheckAgainst(enemyShape);
        for (size_t i = 0; i < collisions.size(); ++i)
        {
            playerBulletPool->ReleaseBullet(collisions[i]);
            if (mEnemy->ReceiveDamage(1))
            {
            }
        }
        mTimer -= dt;
        if (mEnemy->IsDead())
            return false;
        if (mTimer <= 0.0f)
            return false;
        return true;
    }

    virtual void Render() const {}
    virtual void GUI() const {}
    float mTimer;

    std::unique_ptr<Shooter::Enemy> mEnemy;
    std::unique_ptr<Shooter::Player> mPlayer;
    std::unique_ptr<Graphics::AbstractTextureFactory> mTextureFactory;
    std::unique_ptr<Graphics::AsepriteAnimationFactory> mAnimationFactory;
};

TEST_CASE("Game::1- Enemy dies")
{
    // Init test
    Engine e = Engine();
    e.GetCatalog()->OpenPack("TestData");
    Input::VirtualActionManager *vam = new Input::VirtualActionManager();
    e.Setup(std::make_unique<Graphics::DummyRenderer>(), std::unique_ptr<Input::VirtualActionManager>(vam), std::make_unique<Audio::DummyAudioManager>());
    TestLayer *testLayer = new TestLayer();
    e.PushLayer(testLayer);

    // Set specific state
    testLayer->mPlayer->SetPosition({-2.0f, 0.0f});
    testLayer->mEnemy->SetPosition({2.0f, 0.0f});
    vam->SetActionState("Shot", 1.0f);

    // Run!
    e.Run();
    CHECK(testLayer->mEnemy->IsDead());
}

TEST_CASE("Game::2- Enemy doesn't die")
{
    // Init test
    Engine e = Engine();
    e.GetCatalog()->OpenPack("TestData");
    Input::VirtualActionManager *vam = new Input::VirtualActionManager();
    e.Setup(std::make_unique<Graphics::DummyRenderer>(), std::unique_ptr<Input::VirtualActionManager>(vam), std::make_unique<Audio::DummyAudioManager>());
    TestLayer *testLayer = new TestLayer();
    e.PushLayer(testLayer);

    // Set specific state
    testLayer->mPlayer->SetPosition({-2.0f, 3.0f});
    testLayer->mEnemy->SetPosition({2.0f, 0.0f});
    vam->SetActionState("Shot", 1.0f);

    // Run!
    e.Run();
    CHECK(!testLayer->mEnemy->IsDead());
}

#include "GameLayer.h"

#include "Packer/PackCatalog.h"
#include "Audio/IAudioManager.h"
#include "Input/IActionManager.h"

namespace Mochi::Platformer
{
    GameLayer::GameLayer() : Layer()
    {
        mCatalog->OpenPack("Data/Game");
        mAudioManager->LoadAudio("Audio.json");
    }

    GameLayer::~GameLayer()
    {
    }

    void GameLayer::InitLayer()
    {
    }

    bool GameLayer::Update(const float &dt)
    {
        if (mActionManager->Performed("Debug1"))
        {
            mAudioManager->PlayOneShot("WilhelmScream");
        }

        return true;
    }

    void GameLayer::Render() const
    {
    }

    void GameLayer::GUI()
    {
    }
}
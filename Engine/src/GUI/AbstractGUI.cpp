#include "AbstractGUI.h"

#include "../Graphics/IRenderer.h"
#include "../Packer/PackCatalog.h"
#include "../Input/IActionManager.h"
#include "../Graphics/AbstractTextureFactory.h"

#include "../Utils/Logger.h"

namespace Mochi::Graphics
{
    AbstractGUI::AbstractGUI(FS::PackCatalog *catalog, IRenderer *renderer, Input::IActionManager *actionManager)
        : mRenderer(renderer), mCatalog(catalog), mActionManager(actionManager), mCurrentId(0), mFocusId(-1), mPressedId(-1),
          mRaycastBlockingRegions()
    {
        mTextureFactory = mRenderer->CreateTextureFactory(catalog);
    }

    AbstractGUI::~AbstractGUI()
    {
    }

    GUIElementId AbstractGUI::GetNextId()
    {
        return mCurrentId++;
    }

    void AbstractGUI::ResetFrame()
    {
        mCurrentId = 0;
        mRaycastBlockingRegions.clear();
    }

    void AbstractGUI::Update(const float &dt)
    {
        auto const &vertical = mActionManager->Value("UINavigateVertical");
        if (vertical > 0.0f)
        {
            mFocusId--;
        }
        else if (vertical < 0.0f)
        {
            mFocusId++;
        }
        if (mFocusId < 0)
        {
            mFocusId = mCurrentId - 1;
        }
        else if (mFocusId >= mCurrentId)
        {
            mFocusId = 0;
        }

        auto mousePos = mActionManager->CompoundValue("MousePosX", "MousePosY");
        for (auto &region : mRaycastBlockingRegions)
        {
            if (region.IsPointInside(mousePos))
            {
                mActionManager->SetBlockingLayer(0);
                break;
            }
        }

        ResetFrame();
        OnUpdate();
    }
}
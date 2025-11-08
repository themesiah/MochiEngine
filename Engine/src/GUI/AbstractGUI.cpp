#include "AbstractGUI.h"

#include "../Graphics/IRenderer.h"
#include "../Packer/PackCatalog.h"
#include "../Input/IActionManager.h"
#include "../Graphics/AbstractTextureFactory.h"

#include "../Utils/Logger.h"

namespace Mochi::Graphics
{
    AbstractGUI::AbstractGUI(FS::PackCatalog *catalog, IRenderer *renderer, Input::IActionManager *actionManager)
        : mRenderer(renderer), mCatalog(catalog), mActionManager(actionManager)
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
        ResetFrame();
        OnUpdate();
    }
}
#include "AbstractGUI.h"

#include "../Graphics/IRenderer.h"
#include "../Packer/PackCatalog.h"
#include "../Input/IActionManager.h"
#include "../Graphics/AbstractTextureFactory.h"

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
}
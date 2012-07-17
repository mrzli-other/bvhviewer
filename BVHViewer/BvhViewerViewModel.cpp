#include "BvhViewerViewModel.h"
#include "CEGUICommonFileDialog.h"

namespace BVHViewer_Application
{
    BvhViewerViewModel::BvhViewerViewModel(BvhViewerApp *app) :
        mApp(app),
        mIsControlsVisible(false),
        mIsAboutVisible(false)
    {
    }

    BvhViewerViewModel::~BvhViewerViewModel()
    {
    }

    void BvhViewerViewModel::updateFrame()
    {
        mApp->setExecutionSuspended(isAnyDialogActive());
    }

    bool BvhViewerViewModel::isOpenFileVisible() const
    {
        return CEGUI::CommonFileDialog::getSingleton().getWindow()->isVisible();
    }

    bool BvhViewerViewModel::isAnyDialogActive()
    {
        return isControlsVisible() || isAboutVisible() || isOpenFileVisible();
    }

    /* EVENT HANDLERS */
    bool BvhViewerViewModel::openHandler()
    {
        CEGUI::CommonFileDialogInput input;
        input.setHandle(1); // set the handle to determine the originator
        input.setFilter("Biovision hierarchy file (*.bvh)|*.bvh|All files|*.*|"); // extensions
        input.setOpenFileDialog(true); // load mode
        input.setDefaultExtension("*.bvh"); // bvh is the first one to display
        CEGUI::CommonFileDialog::getSingleton().openDialog(input); // open the common file dialog
        return true;
    }

    bool BvhViewerViewModel::fileDialogOutputHandler()
    {
        CEGUI::CommonFileDialogOutput result = CEGUI::CommonFileDialog::getSingleton().getResult();

        if (result.getAction() == CEGUI::ACTION_OK)
        {
            string fileName(result.getFullQualifiedFileName().c_str());
            mApp->openFile(fileName);
        }

        return true;
    }

    bool BvhViewerViewModel::exitHandler()
    {
        mApp->requestShutdown();
        return true;
    }

    bool BvhViewerViewModel::playAnimationHandler()
    {
        if (!canPlayAnimation())
        {
            return false;
        }

        mApp->playAnimation();
        return true;
    }

    bool BvhViewerViewModel::canPlayAnimation()
    {
        return mApp->isDataLoaded() && mApp->getFrameCount() > 0;
    }

    bool BvhViewerViewModel::pauseAnimationHandler()
    {
        if (!canPauseAnimation())
        {
            return false;
        }

        mApp->pauseAnimation();
        return true;
    }

    bool BvhViewerViewModel::canPauseAnimation()
    {
        return mApp->isDataLoaded() && mApp->getFrameCount() > 0 && mApp->isPlaying();
    }

    bool BvhViewerViewModel::resetAnimationHandler()
    {
        if (!canResetAnimation())
        {
            return false;
        }

        mApp->resetAnimation();
        return true;
    }

    bool BvhViewerViewModel::canResetAnimation()
    {
        return mApp->isDataLoaded() && mApp->getFrameCount() > 0 && mApp->getCurrentFrame() > 0;
    }

    bool BvhViewerViewModel::meshSelectHandler(const string &meshName)
    {
        const vector<string> &meshNames = getMeshNames();
        for (int i = 0; i < (int)meshNames.size(); i++)
        {
            if (meshNames[i] == meshName)
            {
                mApp->selectMesh(i);
                return true;
            }
        }

        return true;
    }

    bool BvhViewerViewModel::openControlsHandler()
    {
        if (!canOpenControls())
        {
            return false;
        }

        mIsControlsVisible = true;
        return true;
    }

    bool BvhViewerViewModel::canOpenControls()
    {
        return !mIsControlsVisible;
    }

    bool BvhViewerViewModel::closeControlsHandler()
    {
        if (!canCloseControls())
        {
            return false;
        }

        mIsControlsVisible = false;
        return true;
    }

    bool BvhViewerViewModel::canCloseControls()
    {
        return mIsControlsVisible;
    }

    bool BvhViewerViewModel::openAboutHandler()
    {
        if (!canOpenAbout())
        {
            return false;
        }

        mIsAboutVisible = true;
        return true;
    }

    bool BvhViewerViewModel::canOpenAbout()
    {
        return !mIsAboutVisible;
    }

    bool BvhViewerViewModel::closeAboutHandler()
    {
        if (!canCloseAbout())
        {
            return false;
        }

        mIsAboutVisible = false;
        return true;
    }

    bool BvhViewerViewModel::canCloseAbout()
    {
        return mIsAboutVisible;
    }

    bool BvhViewerViewModel::frameScrollPositionChangedHandler(int scrollPosition)
    {
        if (!canChangeFrame())
        {
            return false;
        }

        mApp->setCurrentFrame(scrollPosition);
        return true;
    }

    bool BvhViewerViewModel::canChangeFrame()
    {
        return mApp->isDataLoaded() && mApp->getFrameCount() > 0;
    }
}
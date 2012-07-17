#ifndef BVH_VIEWER_VIEW_MODEL_H
#define BVH_VIEWER_VIEW_MODEL_H

#include <vector>
#include <string>
#include <CEGUI.h>
#include "BvhViewerApp.h"

namespace BVHViewer_Application
{
    using std::vector;
    using std::string;

    class BvhViewerViewModel
    {
    // ===========================================================
    // Constants
    // ===========================================================

    // ===========================================================
    // Fields
    // ===========================================================
    private:
        BvhViewerApp *mApp;

        bool mIsControlsVisible;
        bool mIsAboutVisible;

    // ===========================================================
    // Constructors/Destructors
    // ===========================================================
    public:
        BvhViewerViewModel(BvhViewerApp *app);
        ~BvhViewerViewModel();

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================

    // ===========================================================
    // Methods
    // ===========================================================
    public:
        const vector<string> &getMeshNames() const { return mApp->getMeshNames(); }
        int getCurrentFrame() const { return mApp->getCurrentFrame(); }
        int getFrameCount() const { return mApp->getFrameCount(); }

        bool isControlsVisible() const { return mIsControlsVisible; }
        bool isAboutVisible() const { return mIsAboutVisible; }
        bool isOpenFileVisible() const;

        void updateFrame();
        bool isAnyDialogActive();

        bool openHandler();
        bool fileDialogOutputHandler();
        bool exitHandler();

        bool playAnimationHandler();
        bool canPlayAnimation();
        bool pauseAnimationHandler();
        bool canPauseAnimation();
        bool resetAnimationHandler();
        bool canResetAnimation();

        bool meshSelectHandler(const string &meshName);

        bool openControlsHandler();
        bool canOpenControls();
        bool closeControlsHandler();
        bool canCloseControls();

        bool openAboutHandler();
        bool canOpenAbout();
        bool closeAboutHandler();
        bool canCloseAbout();

        bool frameScrollPositionChangedHandler(int scrollPosition);
        bool canChangeFrame();

    // ===========================================================
    // Inner and Anonymous Classes
    // ===========================================================
    
    // ===========================================================
    // Getter & Setter
    // ===========================================================
    };
}

#endif
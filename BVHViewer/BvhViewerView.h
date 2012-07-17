#ifndef BVH_VIEWER_VIEW_H
#define BVH_VIEWER_VIEW_H

#include <string>
#include <CEGUI.h>
#include "BvhViewerViewModel.h"

namespace BVHViewer_Application
{
    using std::string;

    class BvhViewerView
    {
    // ===========================================================
    // Constants
    // ===========================================================
    private:
        static const string BVH_VIEWER;

        static const string MENU_FILE_OPEN;
        static const string MENU_FILE_EXIT;
        static const string MENU_ANIMATION_PLAY;
        static const string MENU_ANIMATION_PAUSE;
        static const string MENU_ANIMATION_RESET;
        static const string MENU_MESH__AUTOPUPUP_;
        static const string MENU_HELP_CONTROLS;
        static const string MENU_HELP_ABOUT;

        static const string BUTTON_PLAY;
        static const string BUTTON_PAUSE;
        static const string BUTTON_RESET;
        static const string SLIDER_FRAME;
        static const string TEXT_FRAME;

        static const string CONTROLS_DIALOG;
        static const string CONTROLS_DIALOG_BUTTON_CLOSE;

        static const string ABOUT_DIALOG;
        static const string ABOUT_DIALOG_BUTTON_CLOSE;

    // ===========================================================
    // Fields
    // ===========================================================
    private:
        BvhViewerViewModel *mViewModel;

    // ===========================================================
    // Constructors/Destructors
    // ===========================================================
    public:
        BvhViewerView(BvhViewerViewModel *viewModel);
        ~BvhViewerView();

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================

    // ===========================================================
    // Methods
    // ===========================================================
    public:
        void updateFrame();

    private:
        void initMeshMenu();
        void setEventHandlers();

        bool openHandler(const CEGUI::EventArgs& e);
        bool fileDialogOutputHandler(const CEGUI::EventArgs& e);
        bool exitHandler(const CEGUI::EventArgs& e);

        bool playAnimationHandler(const CEGUI::EventArgs& e);
        bool canPlayAnimation();
        bool pauseAnimationHandler(const CEGUI::EventArgs& e);
        bool canPauseAnimation();
        bool resetAnimationHandler(const CEGUI::EventArgs& e);
        bool canResetAnimation();

        bool meshSelectHandler(const CEGUI::EventArgs& e);

        bool openControlsHandler(const CEGUI::EventArgs& e);
        bool canOpenControls();
        bool closeControlsHandler(const CEGUI::EventArgs& e);
        bool canCloseControls();

        bool openAboutHandler(const CEGUI::EventArgs& e);
        bool canOpenAbout();
        bool closeAboutHandler(const CEGUI::EventArgs& e);
        bool canCloseAbout();

        bool frameScrollPositionChangedHandler(const CEGUI::EventArgs& e);
        bool canChangeFrame();

        CEGUI::Window *getWindow(const string &name);
        bool isAnyDialogActive();

    // ===========================================================
    // Inner and Anonymous Classes
    // ===========================================================
    
    // ===========================================================
    // Getter & Setter
    // ===========================================================
    };
}

#endif
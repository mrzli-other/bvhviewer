#include "BvhViewerView.h"
#include "CEGUICommonFileDialog.h"
#include <sstream>

namespace BVHViewer_Application
{
    using std::stringstream;

    const string BvhViewerView::BVH_VIEWER = "BvhReaderGui";

    const string BvhViewerView::MENU_FILE_OPEN = "BvhReaderGui/MainMenu/File/AutoPopup/Open";
    const string BvhViewerView::MENU_FILE_EXIT = "BvhReaderGui/MainMenu/File/AutoPopup/Exit";
    const string BvhViewerView::MENU_ANIMATION_PLAY = "BvhReaderGui/MainMenu/Animation/AutoPopup/Play";
    const string BvhViewerView::MENU_ANIMATION_PAUSE = "BvhReaderGui/MainMenu/Animation/AutoPopup/Pause";
    const string BvhViewerView::MENU_ANIMATION_RESET = "BvhReaderGui/MainMenu/Animation/AutoPopup/Reset";
    const string BvhViewerView::MENU_MESH__AUTOPUPUP_ = "BvhReaderGui/MainMenu/Mesh/AutoPopup";
    const string BvhViewerView::MENU_HELP_CONTROLS = "BvhReaderGui/MainMenu/Help/AutoPopup/Controls";
    const string BvhViewerView::MENU_HELP_ABOUT = "BvhReaderGui/MainMenu/Help/AutoPopup/About";

    const string BvhViewerView::BUTTON_PLAY = "BvhReaderGui/HorizontalLayoutContainer/Play";
    const string BvhViewerView::BUTTON_PAUSE = "BvhReaderGui/HorizontalLayoutContainer/Pause";
    const string BvhViewerView::BUTTON_RESET = "BvhReaderGui/HorizontalLayoutContainer/Reset";
    const string BvhViewerView::SLIDER_FRAME = "BvhReaderGui/HorizontalLayoutContainer/FrameSlider";
    const string BvhViewerView::TEXT_FRAME = "BvhReaderGui/HorizontalLayoutContainer/FrameText";

    const string BvhViewerView::CONTROLS_DIALOG = "ControlsWindow";
    const string BvhViewerView::CONTROLS_DIALOG_BUTTON_CLOSE = "ControlsWindow/CloseButton";

    const string BvhViewerView::ABOUT_DIALOG = "AboutWindow";
    const string BvhViewerView::ABOUT_DIALOG_BUTTON_CLOSE = "AboutWindow/CloseButton";

    //-------------------------------------------------------------------------------------
    BvhViewerView::BvhViewerView(BvhViewerViewModel *viewModel) :
        mViewModel(viewModel)
    {
        CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();

        CEGUI::Window* guiRoot = winMgr.createWindow("DefaultWindow", "GuiRoot");
        guiRoot->setProperty("MousePassThroughEnabled", "True");
        CEGUI::System::getSingleton().setGUISheet(guiRoot);

        CEGUI::Window *bvhViewer = winMgr.loadWindowLayout("BvhReader.layout");
        guiRoot->addChildWindow(bvhViewer);

        guiRoot->addChildWindow(CEGUI::CommonFileDialog::getSingleton().getWindow());

        CEGUI::Window *aboutWindow = winMgr.loadWindowLayout("AboutWindow.layout");
        guiRoot->addChildWindow(aboutWindow);
        aboutWindow->setVisible(false);

        CEGUI::Window *controlsWindow = winMgr.loadWindowLayout("ControlsWindow.layout");
        guiRoot->addChildWindow(controlsWindow);
        controlsWindow->setVisible(false);

        initMeshMenu();
        setEventHandlers();
    }

    //-------------------------------------------------------------------------------------
    BvhViewerView::~BvhViewerView()
    {
        if (mViewModel != NULL)
        {
            delete mViewModel;
            mViewModel = NULL;
        }
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerView::initMeshMenu()
    {
        CEGUI::MenuItem *meshBaseMenuItem = (CEGUI::MenuItem *)CEGUI::WindowManager::getSingleton().getWindow("BvhReaderGui/MainMenu/Mesh");
        CEGUI::PopupMenu *popupMenu = (CEGUI::PopupMenu *)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/PopupMenu", MENU_MESH__AUTOPUPUP_);
        meshBaseMenuItem->addChildWindow(popupMenu);

        const vector<string> &meshNames = mViewModel->getMeshNames();
        vector<string>::const_iterator it, itEnd = meshNames.end();
        for (it = meshNames.begin(); it != itEnd; it++)
        {
            CEGUI::MenuItem *menuItem = (CEGUI::MenuItem *)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/MenuItem", "BvhReaderGui/MainMenu/Mesh/" + *it);
            menuItem->setText(*it);
            popupMenu->addChildWindow(menuItem);
        }
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerView::setEventHandlers()
    {
        // menu file
        getWindow(MENU_FILE_OPEN)->subscribeEvent(
            CEGUI::PushButton::EventClicked, 
            CEGUI::Event::Subscriber(&BvhViewerView::openHandler, this));

        getWindow(MENU_FILE_EXIT)->subscribeEvent(
            CEGUI::PushButton::EventClicked, 
            CEGUI::Event::Subscriber(&BvhViewerView::exitHandler, this));

        // menu animation
        getWindow(MENU_ANIMATION_PLAY)->subscribeEvent(
            CEGUI::PushButton::EventClicked, 
            CEGUI::Event::Subscriber(&BvhViewerView::playAnimationHandler, this));

        getWindow(MENU_ANIMATION_PAUSE)->subscribeEvent(
            CEGUI::PushButton::EventClicked, 
            CEGUI::Event::Subscriber(&BvhViewerView::pauseAnimationHandler, this));

        getWindow(MENU_ANIMATION_RESET)->subscribeEvent(
            CEGUI::PushButton::EventClicked, 
            CEGUI::Event::Subscriber(&BvhViewerView::resetAnimationHandler, this));

        // menu mesh
        CEGUI::PopupMenu *meshPopup = (CEGUI::PopupMenu *)getWindow(MENU_MESH__AUTOPUPUP_);
        for (int i = 0; i < (int)meshPopup->getChildCount(); i++)
        {
            ((CEGUI::MenuItem *)meshPopup->getChildAtIdx(i))->subscribeEvent(
                CEGUI::PushButton::EventClicked,
                CEGUI::Event::Subscriber(&BvhViewerView::meshSelectHandler, this));
        }

        // menu help
        getWindow(MENU_HELP_CONTROLS)->subscribeEvent(
            CEGUI::PushButton::EventClicked, 
            CEGUI::Event::Subscriber(&BvhViewerView::openControlsHandler, this));

        getWindow(MENU_HELP_ABOUT)->subscribeEvent(
            CEGUI::PushButton::EventClicked, 
            CEGUI::Event::Subscriber(&BvhViewerView::openAboutHandler, this));

        // bottom toolbar
        getWindow(BUTTON_PLAY)->subscribeEvent(
            CEGUI::PushButton::EventClicked, 
            CEGUI::Event::Subscriber(&BvhViewerView::playAnimationHandler, this));

        getWindow(BUTTON_PAUSE)->subscribeEvent(
            CEGUI::PushButton::EventClicked, 
            CEGUI::Event::Subscriber(&BvhViewerView::pauseAnimationHandler, this));

        getWindow(BUTTON_RESET)->subscribeEvent(
            CEGUI::PushButton::EventClicked, 
            CEGUI::Event::Subscriber(&BvhViewerView::resetAnimationHandler, this));

        getWindow(SLIDER_FRAME)->subscribeEvent(
            CEGUI::Scrollbar::EventScrollPositionChanged,
            CEGUI::Event::Subscriber(&BvhViewerView::frameScrollPositionChangedHandler, this));

        // open file dialog
        CEGUI::CommonFileDialog::getSingleton().getWindow()->subscribeEvent(
            CEGUI::FrameWindow::EventHidden,
            CEGUI::Event::Subscriber(&BvhViewerView::fileDialogOutputHandler, this));

        // controls dialog
        getWindow(CONTROLS_DIALOG_BUTTON_CLOSE)->subscribeEvent(
            CEGUI::PushButton::EventClicked, 
            CEGUI::Event::Subscriber(&BvhViewerView::closeControlsHandler, this));

        ((CEGUI::FrameWindow *)getWindow(CONTROLS_DIALOG))->getCloseButton()->subscribeEvent(
            CEGUI::PushButton::EventClicked, 
            CEGUI::Event::Subscriber(&BvhViewerView::closeControlsHandler, this));

        // about dialog
        getWindow(ABOUT_DIALOG_BUTTON_CLOSE)->subscribeEvent(
            CEGUI::PushButton::EventClicked, 
            CEGUI::Event::Subscriber(&BvhViewerView::closeAboutHandler, this));

        ((CEGUI::FrameWindow *)getWindow(ABOUT_DIALOG))->getCloseButton()->subscribeEvent(
            CEGUI::PushButton::EventClicked, 
            CEGUI::Event::Subscriber(&BvhViewerView::closeAboutHandler, this));
    }

    void BvhViewerView::updateFrame()
    {
        mViewModel->updateFrame();

        // main layout
        getWindow(BVH_VIEWER)->setEnabled(!isAnyDialogActive());

        // menu file
        getWindow(MENU_FILE_OPEN)->setEnabled(!mViewModel->isOpenFileVisible());

        // menu animation
        getWindow(MENU_ANIMATION_PLAY)->setEnabled(canPlayAnimation());
        getWindow(MENU_ANIMATION_PAUSE)->setEnabled(canPauseAnimation());
        getWindow(MENU_ANIMATION_RESET)->setEnabled(canResetAnimation());

        // menu help
        getWindow(MENU_HELP_CONTROLS)->setEnabled(!mViewModel->isControlsVisible());
        getWindow(MENU_HELP_ABOUT)->setEnabled(!mViewModel->isAboutVisible());

        // buttons
        getWindow(BUTTON_PLAY)->setEnabled(canPlayAnimation());
        getWindow(BUTTON_PAUSE)->setEnabled(canPauseAnimation());
        getWindow(BUTTON_RESET)->setEnabled(canResetAnimation());

        // frame slider and text
        CEGUI::Scrollbar *frameSlider = (CEGUI::Scrollbar *)getWindow(SLIDER_FRAME);
        CEGUI::DefaultWindow *frameText = (CEGUI::DefaultWindow *)getWindow(TEXT_FRAME);
        if (canChangeFrame())
        {
            stringstream ss;
            int currentFrame = mViewModel->getCurrentFrame();
            int frameCount = mViewModel->getFrameCount();

            frameSlider->setEnabled(true);
            frameSlider->setDocumentSize(frameCount - 1);
            frameSlider->setScrollPosition(currentFrame);
            ss << currentFrame + 1 << "/" << frameCount;
            frameSlider->setTooltipText(ss.str());
            
            ss.str("");
            ss.clear();
            ss << "Frame: " << currentFrame + 1 << "/" << frameCount;
            frameText->setText(ss.str());
        }
        else
        {
            frameSlider->setEnabled(false);
            frameSlider->setDocumentSize(1);
            frameSlider->setScrollPosition(0);
            frameSlider->setTooltip(NULL);

            frameText->setText("Frame: - ");
        }

        // controls dialog
        getWindow(CONTROLS_DIALOG)->setVisible(mViewModel->isControlsVisible());

        // about dialog
        getWindow(ABOUT_DIALOG)->setVisible(mViewModel->isAboutVisible());
    }

    CEGUI::Window *BvhViewerView::getWindow(const string &name)
    {
        CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();
        return winMgr.getWindow(name);
    }

    bool BvhViewerView::isAnyDialogActive()
    {
        return mViewModel->isControlsVisible() || mViewModel->isAboutVisible() || mViewModel->isOpenFileVisible();
    }

    /* EVENT HANDLERS */
    bool BvhViewerView::openHandler(const CEGUI::EventArgs& e)
    {
        return mViewModel->openHandler();
    }

    bool BvhViewerView::fileDialogOutputHandler(const CEGUI::EventArgs& e)
    {
        return mViewModel->fileDialogOutputHandler();
    }

    bool BvhViewerView::exitHandler(const CEGUI::EventArgs& e)
    {
        return mViewModel->exitHandler();
    }

    bool BvhViewerView::playAnimationHandler(const CEGUI::EventArgs& e)
    {
        if (!canPlayAnimation())
        {
            return false;
        }

        return mViewModel->playAnimationHandler();
    }

    bool BvhViewerView::canPlayAnimation()
    {
        return mViewModel->canPlayAnimation();
    }

    bool BvhViewerView::pauseAnimationHandler(const CEGUI::EventArgs& e)
    {
        if (!canPauseAnimation())
        {
            return false;
        }

        return mViewModel->pauseAnimationHandler();
    }

    bool BvhViewerView::canPauseAnimation()
    {
        return mViewModel->canPauseAnimation();
    }

    bool BvhViewerView::resetAnimationHandler(const CEGUI::EventArgs& e)
    {
        if (!canResetAnimation())
        {
            return false;
        }

        return mViewModel->resetAnimationHandler();
    }

    bool BvhViewerView::canResetAnimation()
    {
        return mViewModel->canResetAnimation();
    }

    bool BvhViewerView::meshSelectHandler(const CEGUI::EventArgs& e)
    {
        CEGUI::Window* window = ((const CEGUI::WindowEventArgs&)e).window;
        return mViewModel->meshSelectHandler(window->getText().c_str());
    }

    bool BvhViewerView::openControlsHandler(const CEGUI::EventArgs& e)
    {
        if (!canOpenControls())
        {
            return false;
        }

        return mViewModel->openControlsHandler();
    }

    bool BvhViewerView::canOpenControls()
    {
        return mViewModel->canOpenControls();
    }

    bool BvhViewerView::closeControlsHandler(const CEGUI::EventArgs& e)
    {
        if (!canCloseControls())
        {
            return false;
        }

        return mViewModel->closeControlsHandler();
    }

    bool BvhViewerView::canCloseControls()
    {
        return mViewModel->canCloseControls();
    }

    bool BvhViewerView::openAboutHandler(const CEGUI::EventArgs& e)
    {
        if (!canOpenAbout())
        {
            return false;
        }

        return mViewModel->openAboutHandler();
    }

    bool BvhViewerView::canOpenAbout()
    {
        return mViewModel->canOpenAbout();
    }

    bool BvhViewerView::closeAboutHandler(const CEGUI::EventArgs& e)
    {
        if (!canCloseAbout())
        {
            return false;
        }

        return mViewModel->closeAboutHandler();
    }

    bool BvhViewerView::canCloseAbout()
    {
        return mViewModel->canCloseAbout();
    }

    bool BvhViewerView::frameScrollPositionChangedHandler(const CEGUI::EventArgs& e)
    {
        if (!canChangeFrame())
        {
            return false;
        }

        CEGUI::Scrollbar* frameScroll = (CEGUI::Scrollbar *)((const CEGUI::WindowEventArgs&)e).window;
        int scrollPosition = (int)frameScroll->getScrollPosition();

        return mViewModel->frameScrollPositionChangedHandler(scrollPosition);
    }

    bool BvhViewerView::canChangeFrame()
    {
        return mViewModel->canChangeFrame();
    }
    /* END EVENT HANDLERS */
}
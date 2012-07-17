/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef TUTORIAL_APPLICATION_H
#define TUTORIAL_APPLICATION_H

#include <vector>
#include <string>
#include <map>

#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>

#include "BaseApplication.h"
#include "BvhData.h"
#include "BoneInfo.h"
#include "AnimationData.h"

namespace BVHViewer_Application
{
    using namespace BVHViewer_DataObjects;
    using namespace BVHViewer_Data;
    using std::vector;
    using std::string;
    using std::map;

    class BvhViewerView;

    class BvhViewerApp : public BaseApplication
    {
    // ===========================================================
    // Constants
    // ===========================================================
    private:
        static const string CONTAINER_NODE_NAME;

    // ===========================================================
    // Fields
    // ===========================================================

    private:
        CEGUI::OgreRenderer *mRenderer;
        CEGUI::Point mSavedMousePosition;

        Ogre::Timer *mTimer;

        BvhViewerView *mView;

        vector<string> mMeshNames;
        int mCurrentMeshIndex;

        map<int, BoneInfo> mBoneInfos;

        AnimationData *mAnimationData;

        bool mIsPlaying;
        int mCurrentFrame;

        float mYaw;
        float mPitch;

        bool mIsExecutionSuspended;

    // ===========================================================
    // Constructors/Destructors
    // ===========================================================

    public:
        BvhViewerApp(void);
        virtual ~BvhViewerApp(void);

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================

    protected:
        virtual void setupResources(void);
        virtual void createCamera(void);
        virtual void createViewports(void);
        virtual void createFrameListener(void);

        virtual void createScene(void);

        // Ogre::FrameListener
        virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

        // OIS::KeyListener
        virtual bool keyPressed( const OIS::KeyEvent &arg );
        virtual bool keyReleased( const OIS::KeyEvent &arg );
        // OIS::MouseListener
        virtual bool mouseMoved( const OIS::MouseEvent &arg );
        virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
        virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // ===========================================================
    // Methods
    // ===========================================================
    public:
        bool openFile(const string &filePath);

        void requestShutdown();

        void playAnimation();
        void pauseAnimation();
        void resetAnimation();

        void selectMesh(int index);
        void setCurrentFrame(int frame);

        const vector<string> &getMeshNames() const { return mMeshNames; }
        bool isDataLoaded() const { return mAnimationData != NULL; }
        bool isPlaying() const { return mIsPlaying; }
        int getCurrentFrame() const { return mCurrentFrame; }
        int getFrameCount() const { return mAnimationData != NULL ? mAnimationData->getFrameCount() : 0; }

        void setExecutionSuspended(bool isSuspended) { mIsExecutionSuspended = isSuspended; }

    private:
        void release();

        void resetCamera();
        void setLights();
        void createGrid();

        void initData();
        void clearData();
        void createSkeletons(const BvhData *bvhData);
        void setMeshToBones(int meshIndex);
        void scaleBoneModels();
        void applyFrame(int frame);

        bool isLeftActive();
        bool isRightActive();
        bool isForwardActive();
        bool isBackActive();
        bool isUpActive();
        bool isDownActive();

        bool isLeftKeyActive();
        bool isRightKeyActive();
        bool isForwardKeyActive();
        bool isBackKeyActive();
        bool isUpKeyActive();
        bool isDownKeyActive();

    // ===========================================================
    // Inner and Anonymous Classes
    // ===========================================================
    
    // ===========================================================
    // Getter & Setter
    // ===========================================================
    };
}

#endif // #ifndef TUTORIAL_APPLICATION_H
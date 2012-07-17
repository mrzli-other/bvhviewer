/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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

#include "BvhViewerApp.h"
#include "CEGUICommonFileDialog.h"
#include "BvhReader.h"
#include "AnimationData.h"
#include "BoneInfo.h"
#include "BvhViewerView.h"

#include <sstream>

namespace BVHViewer_Application
{
    using namespace BVHViewer_DataAccess;
    using namespace BVHViewer_DataObjects;
    using namespace BVHViewer_Data;

    using std::stringstream;
    using std::map;

    const string BvhViewerApp::CONTAINER_NODE_NAME = "Bone Nodes Container";

    CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);
    void raiseException(string text);

    //-------------------------------------------------------------------------------------
    BvhViewerApp::BvhViewerApp(void) :
        mView(NULL),
        mAnimationData(NULL),
        mIsPlaying(false),
        mCurrentFrame(0),
        mYaw(0.0f),
        mPitch(0.0f),
        mIsExecutionSuspended(false)
    {
        mTimer = new Ogre::Timer();
        mSavedMousePosition = CEGUI::Point();
    }

    //-------------------------------------------------------------------------------------
    BvhViewerApp::~BvhViewerApp(void)
    {
        release();
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::release()
    {
        clearData();

        if (mView != NULL)
        {
            delete mView;
            mView = NULL;
        }

        delete mTimer;
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::setupResources(void)
    {
        mMeshNames.clear();

        // Load resource paths from config file
        Ogre::ConfigFile cf;
        cf.load(mResourcesCfg);

        // Go through all sections & settings in the file
        Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

        Ogre::String secName, typeName, archName;
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
            Ogre::ConfigFile::SettingsMultiMap::iterator i;

            Ogre::Archive* archive = Ogre::ArchiveManager::getSingleton().load("..\\Data\\Meshes", "FileSystem");

            for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);

                if (secName == "Meshes") {
                    Ogre::Archive* archive = Ogre::ArchiveManager::getSingleton().load(archName, typeName);
                    Ogre::StringVectorPtr vec = archive->find("*", false);
                    for (Ogre::StringVector::iterator it = vec->begin(); it != vec->end(); ++it)
                    {
                        mMeshNames.push_back(*it);
                    }

                    mCurrentMeshIndex = 0;
                }
            }
        }
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::createCamera(void)
    {
        // Create the camera
        mCamera = mSceneMgr->createCamera("Camera");
        mCamera->setNearClipDistance(5);

        resetCamera();
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::createViewports(void)
    {
        // Create one viewport, entire window
        Ogre::Viewport* vp = mWindow->addViewport(mCamera);
        vp->setBackgroundColour(Ogre::ColourValue(0.8f, 0.8f, 0.8f));

        // Alter the camera aspect ratio to match the viewport
        mCamera->setAspectRatio(
            Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::createFrameListener(void)
    {
        Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
        OIS::ParamList pl;
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;

        mWindow->getCustomAttribute("WINDOW", &windowHnd);
        windowHndStr << windowHnd;
        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

        mInputManager = OIS::InputManager::createInputSystem(pl);

        mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
        mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

        mMouse->setEventCallback(this);
        mKeyboard->setEventCallback(this);

        //Set initial mouse clipping size
        windowResized(mWindow);

        //Register as a Window listener
        Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

        mRoot->addFrameListener(this);
    }
    
    //-------------------------------------------------------------------------------------
    void BvhViewerApp::createScene(void)
    {
        mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

        CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
        CEGUI::Font::setDefaultResourceGroup("Fonts");
        CEGUI::Scheme::setDefaultResourceGroup("Schemes");
        CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
        CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

        CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");

        CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

        CEGUI::FontManager::getSingleton().create("BlueHighway-10.font");
        CEGUI::System::getSingleton().setDefaultFont("BlueHighway-10");

        mView = new BvhViewerView(new BvhViewerViewModel(this));

        setLights();
        createGrid();
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::resetCamera()
    {
        mYaw = 0.0f;
        mPitch = 0.0f;

        mCamera->setPosition(Ogre::Vector3(0.0f, 30.0f, 200.0f));
        mCamera->setDirection(-Ogre::Vector3::UNIT_Z);
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::setLights()
    {
        // set ambient light
        mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

        // create light
        Ogre::Light *light;
        light = mSceneMgr->createLight("Light");
        light->setType(Ogre::Light::LT_DIRECTIONAL);
        light->setDiffuseColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
        light->setSpecularColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
        light->setDirection(Ogre::Vector3(1.0f, -1.0f, -1.0f));
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::createGrid()
    {
        const float RANGE = 100;
        const float STEP = 10;

        Ogre::ManualObject *manual = mSceneMgr->createManualObject("Grid");

        manual->begin("BVH_Reader/Grid", Ogre::RenderOperation::OT_LINE_LIST);
 
        for (float pos = -RANGE; pos <= RANGE; pos += STEP)
        {
            manual->position(-RANGE, 0, pos);
            manual->position(RANGE, 0, pos);
        }

        for (float pos = -RANGE; pos <= RANGE; pos += STEP)
        {
            manual->position(pos, 0, -RANGE);
            manual->position(pos, 0, RANGE);
        }
 
        manual->end();
 
        mSceneMgr->getRootSceneNode()->attachObject(manual);
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::openFile(const string &filePath)
    {
        initData();
        clearData();

        BvhData *bvhData = new BvhData();

        BvhReader::read(bvhData, filePath);

        createSkeletons(bvhData);
        setMeshToBones(mCurrentMeshIndex);
        scaleBoneModels();

        mAnimationData = new AnimationData(bvhData);

        applyFrame(0);

        // bvhData is deleted when mAnimationData is deleted
        //delete bvhData;

        return true;
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::initData()
    {
        mIsPlaying = false;
        mCurrentFrame = 0;

        // create meshes
        for (int i = 0; i < (int)mMeshNames.size(); i++)
        {
            if (!mSceneMgr->hasEntity(mMeshNames[i]))
            {
                mSceneMgr->createEntity(mMeshNames[i], mMeshNames[i]);
            }
        }

        // create container node for bones
        Ogre::SceneNode *containerNode;
        if (mSceneMgr->hasSceneNode(CONTAINER_NODE_NAME))
        {
            containerNode = mSceneMgr->getSceneNode(CONTAINER_NODE_NAME);
        }
        else
        {
            containerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(CONTAINER_NODE_NAME);
        }
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::clearData()
    {
        if (mSceneMgr != NULL && mSceneMgr->hasSceneNode(CONTAINER_NODE_NAME))
        {
            Ogre::SceneNode *containerNode = mSceneMgr->getSceneNode(CONTAINER_NODE_NAME);

            // destroy objects from previously loaded skeleton
            Ogre::Node::ChildNodeIterator modelNodesIt = containerNode->getChildIterator();
            while (modelNodesIt.hasMoreElements())
            {
                Ogre::SceneNode *modelNode = (Ogre::SceneNode *) modelNodesIt.getNext();

                Ogre::SceneNode::ObjectIterator attachedObjectsIt = modelNode->getAttachedObjectIterator();
                while (attachedObjectsIt.hasMoreElements())
                {
                    Ogre::Entity *pEntity = (Ogre::Entity *) attachedObjectsIt.getNext();
                    modelNode->detachObject(pEntity);
                    mSceneMgr->destroyEntity(pEntity);
                }
            }

            containerNode->removeAndDestroyAllChildren();

            Ogre::SkeletonManager *skelMgr = Ogre::SkeletonManager::getSingletonPtr();
            skelMgr->removeAll();

            // clear bone infos
            mBoneInfos.clear();
        }

        // release animation data
        if (mAnimationData != NULL)
        {
            delete mAnimationData;
            mAnimationData = NULL;
        }
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::createSkeletons(const BvhData *bvhData)
    {
        Ogre::SkeletonManager *skelMgr = Ogre::SkeletonManager::getSingletonPtr();

        vector<BvhHierarchyNode> rootNodes = bvhData->getRootNodes();
        int rootNodesSize = rootNodes.size();
        for (int i = 0; i < rootNodesSize; i++)
        {
            BvhHierarchyNode rootBvhNode = rootNodes[i];

            stringstream ssSkelName;
            ssSkelName << "Skeleton." << i;

            Ogre::SkeletonPtr skeleton = (Ogre::SkeletonPtr) skelMgr->create(ssSkelName.str(), "General");

            stringstream ssBoneName;
            ssBoneName << "Bone." << rootBvhNode.NodeName << "." << rootBvhNode.NodeIndex;
            Ogre::Bone *bone = skeleton->createBone(ssBoneName.str());

            BoneInfo boneInfo;
            boneInfo.SceneNode = NULL;
            boneInfo.SkeletonHandle = skeleton->getHandle();
            boneInfo.BoneHandle = bone->getHandle();
            boneInfo.Length = 0.0f;
            boneInfo.ParentIndex = rootBvhNode.ParentIndex;
            mBoneInfos.insert(make_pair(rootBvhNode.NodeIndex, boneInfo));

            bone->setManuallyControlled(true);
            bone->setInheritOrientation(false);
            bone->setPosition(Ogre::Vector3::ZERO);
            bone->setOrientation(Ogre::Quaternion::IDENTITY);
            bone->setScale(Ogre::Vector3::UNIT_SCALE);
            bone->setInitialState();
        }

        Ogre::SceneNode *containerNode = mSceneMgr->getSceneNode(CONTAINER_NODE_NAME);

        typedef map<int, BoneInfo>::const_iterator bone_info_it;
        int maxParentIndex = bvhData->getMaxParentIndex();
        for (int i = 1; i <= maxParentIndex; i++)
        {
            bone_info_it parentBoneInfoIt = mBoneInfos.find(i);
            if (parentBoneInfoIt == mBoneInfos.end())
            {
                raiseException("Missing parent bone info.");
            }

            BoneInfo parentBoneInfo = parentBoneInfoIt->second;

            Ogre::SkeletonPtr skeleton = skelMgr->getByHandle(parentBoneInfo.SkeletonHandle);
            Ogre::Bone *parentBone = skeleton->getBone(parentBoneInfo.BoneHandle);

            BvhHierarchyNode bvhParentNode = bvhData->getNode(i);
            Ogre::Vector3 parentOffset(bvhParentNode.Offset);

            vector<BvhHierarchyNode> childNodes = bvhData->getChildNodes(i);
            int childNodesSize = childNodes.size();
            for (int j = 0; j < childNodesSize; j++)
            {
                BvhHierarchyNode bvhNode = childNodes[j];

                stringstream ssBoneName;
                ssBoneName << "Bone." << bvhNode.NodeName << "." << bvhNode.NodeIndex;
                Ogre::Bone *bone = skeleton->createBone(ssBoneName.str());
                parentBone->addChild(bone);

                stringstream ssModelName;
                ssModelName << ssBoneName.str() << ".Model";

                Ogre::Vector3 offset(bvhNode.Offset);

                BoneInfo boneInfo;
                boneInfo.SceneNode = containerNode->createChildSceneNode(ssModelName.str());
                boneInfo.SkeletonHandle = parentBoneInfo.SkeletonHandle;
                boneInfo.BoneHandle = bone->getHandle();
                boneInfo.Length = offset.length();
                boneInfo.ParentIndex = i;
                mBoneInfos.insert(make_pair(bvhNode.NodeIndex, boneInfo));

                bone->setManuallyControlled(true);
                bone->setInheritOrientation(false);
                bone->setPosition(0.0, parentOffset.length(), 0.0);
                bone->setOrientation(Ogre::Vector3(Ogre::Vector3::UNIT_Y).getRotationTo(offset));
                bone->setScale(Ogre::Vector3::UNIT_SCALE);
                bone->setInitialState();
            }
        }
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::setMeshToBones(int meshIndex)
    {
        string meshName = mMeshNames[meshIndex];

        Ogre::Entity *boneModel = mSceneMgr->getEntity(meshName);

        Ogre::SceneNode *containerNode = mSceneMgr->getSceneNode(CONTAINER_NODE_NAME);
        Ogre::Node::ChildNodeIterator modelNodesIt = containerNode->getChildIterator();
        while (modelNodesIt.hasMoreElements())
        {
            Ogre::SceneNode *modelNode = (Ogre::SceneNode *) modelNodesIt.getNext();

            // detach object if any attached
            Ogre::SceneNode::ObjectIterator attachedObjectsIt = modelNode->getAttachedObjectIterator();
            while (attachedObjectsIt.hasMoreElements())
            {
                Ogre::Entity *entity = (Ogre::Entity *) attachedObjectsIt.getNext();
                modelNode->detachObject(entity);
                mSceneMgr->destroyEntity(entity);
            }

            // attach object
            modelNode->attachObject(boneModel->clone(modelNode->getName()));
        }
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::scaleBoneModels()
    {
        Ogre::SkeletonManager *skelMgr = Ogre::SkeletonManager::getSingletonPtr();

        typedef map<int, BoneInfo>::const_iterator bone_info_it;
        bone_info_it itEnd = mBoneInfos.end();
        for (bone_info_it it = mBoneInfos.begin(); it != itEnd; it++)
        {
            int nodeIndex = it->first;
            BoneInfo boneInfo = it->second;
            if (boneInfo.SceneNode != NULL)
            {
                boneInfo.SceneNode->scale(boneInfo.Length, boneInfo.Length, boneInfo.Length);
            }
        }
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::applyFrame(int frame)
    {
        if (!isDataLoaded())
        {
            return;
        }

        Ogre::SkeletonManager *skelMgr = Ogre::SkeletonManager::getSingletonPtr();

        typedef map<int, BoneInfo>::const_iterator bone_info_it;
        bone_info_it itEnd = mBoneInfos.end();
        for (bone_info_it it = mBoneInfos.begin(); it != itEnd; it++)
        {
            int nodeIndex = it->first;
            BoneInfo boneInfo = it->second;

            Ogre::SkeletonPtr skeleton = skelMgr->getByHandle(boneInfo.SkeletonHandle);
            Ogre::Bone *bone = skeleton->getBone(boneInfo.BoneHandle);

            bone->setPosition(bone->getInitialPosition());
            bone->setOrientation(bone->getInitialOrientation());

            int frameCount = mAnimationData->getFrameCount();
            if (0 <= frame && frame < frameCount)
            {
                Ogre::Vector3 pos = mAnimationData->getTranslation(nodeIndex, frame);
                bone->translate(pos, Ogre::Node::TS_WORLD);

                Ogre::Quaternion rot = mAnimationData->getRotation(nodeIndex, frame);
                bone->rotate(rot, Ogre::Node::TS_WORLD);
            }

            //if (!boneInfo.pSceneNode)
            //{
            //    m_skeletonPosition = pBone->getWorldPosition();
            //    continue;
            //}

            if (boneInfo.SceneNode != NULL)
            {
                boneInfo.SceneNode->setPosition(bone->_getDerivedPosition());
                boneInfo.SceneNode->setOrientation(bone->_getDerivedOrientation());
            }
        }
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::requestShutdown()
    {
        mShutDown = true;
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::playAnimation()
    {
        if (!isDataLoaded())
        {
            return;
        }

        if (mIsPlaying || mCurrentFrame >= mAnimationData->getFrameCount() - 1)
        {
            resetAnimation();
        }
        mIsPlaying = true;
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::pauseAnimation()
    {
        mIsPlaying = false;
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::resetAnimation()
    {
        mIsPlaying = false;
        setCurrentFrame(0);
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::selectMesh(int index)
    {
        mCurrentMeshIndex = index;
        setMeshToBones(index);
    }

    //-------------------------------------------------------------------------------------
    void BvhViewerApp::setCurrentFrame(int frame)
    {
        if (isDataLoaded() && frame >= 0 && frame < mAnimationData->getFrameCount())
        {
            mCurrentFrame = frame;
            applyFrame(mCurrentFrame);
        }
        else
        {
            mCurrentFrame = 0;
        }
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::isLeftActive()
    {
        return isLeftKeyActive() && !isRightKeyActive();
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::isRightActive()
    {
        return isRightKeyActive() && !isLeftKeyActive();
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::isForwardActive()
    {
        return isForwardKeyActive() && !isBackKeyActive();
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::isBackActive()
    {
        return isBackKeyActive() && !isForwardKeyActive();
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::isUpActive()
    {
        return isUpKeyActive() && !isDownKeyActive();
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::isDownActive()
    {
        return isDownKeyActive() && !isUpKeyActive();
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::isLeftKeyActive()
    {
        return mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_A);
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::isRightKeyActive()
    {
        return mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_D);
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::isForwardKeyActive()
    {
        return mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W);
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::isBackKeyActive()
    {
        return mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S);
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::isUpKeyActive()
    {
        return mKeyboard->isKeyDown(OIS::KC_PGUP) || mKeyboard->isKeyDown(OIS::KC_SPACE);
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::isDownKeyActive()
    {
        return mKeyboard->isKeyDown(OIS::KC_PGDOWN) || mKeyboard->isKeyDown(OIS::KC_LCONTROL);
    }

    /* INPUT EVENT HANDLERS */
    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::frameRenderingQueued(const Ogre::FrameEvent& evt)
    {
        const float CAMERA_MOVE_MULTIPLIER = 50.0f;

        if(mWindow->isClosed())
            return false;

        if(mShutDown)
            return false;

        if (mView != NULL)
        {
            mView->updateFrame();
        }

        //Need to capture/update each device
        mKeyboard->capture();
        mMouse->capture();

        //Need to inject timestamps to CEGUI System.
        CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

        // execution is suspended, skip all animation and camera actions
        // for example, this happend when dialog is active
        if (mIsExecutionSuspended)
        {
            return true;
        }

        static unsigned long prevTime = mTimer->getMilliseconds();

        // just for robustness
        if (!isDataLoaded())
        {
            resetAnimation();
        }

        if (mIsPlaying && isDataLoaded())
        {
            unsigned long currTime = mTimer->getMilliseconds();
            if (currTime - prevTime >= mAnimationData->getFrameTime() * 1000.0f)
            {
                if (mCurrentFrame < 0 || mCurrentFrame >= mAnimationData->getFrameCount())
                {
                    mIsPlaying = false;
                }
                else
                {
                    applyFrame(mCurrentFrame);
                    if (mCurrentFrame < mAnimationData->getFrameCount() - 1)
                    {
                        mCurrentFrame++;
                    }
                    else
                    {
                        mIsPlaying = false;
                    }
                }

                prevTime = currTime;
            }
        }

        // handle camera movement
        if (mCamera != NULL)
        {
            if (isLeftActive())
            {
                mCamera->moveRelative(-Ogre::Vector3::UNIT_X * evt.timeSinceLastFrame * CAMERA_MOVE_MULTIPLIER);
            }
            else if (isRightActive())
            {
                mCamera->moveRelative(Ogre::Vector3::UNIT_X * evt.timeSinceLastFrame * CAMERA_MOVE_MULTIPLIER);
            }

            if (isForwardActive())
            {
                mCamera->moveRelative(-Ogre::Vector3::UNIT_Z * evt.timeSinceLastFrame * CAMERA_MOVE_MULTIPLIER);
            }
            else if (isBackActive())
            {
                mCamera->moveRelative(Ogre::Vector3::UNIT_Z * evt.timeSinceLastFrame * CAMERA_MOVE_MULTIPLIER);
            }

            if (isUpActive())
            {
                mCamera->moveRelative(Ogre::Vector3::UNIT_Y * evt.timeSinceLastFrame * CAMERA_MOVE_MULTIPLIER);
            }
            else if (isDownActive())
            {
                mCamera->moveRelative(-Ogre::Vector3::UNIT_Y * evt.timeSinceLastFrame * CAMERA_MOVE_MULTIPLIER);
            }
        }

        return true;
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::keyPressed( const OIS::KeyEvent &arg )
    {
        if (arg.key == OIS::KC_ESCAPE)
        {
            mShutDown = true;
        }
        else if (arg.key == OIS::KC_R && !mIsExecutionSuspended)
        {
            resetCamera();
        }
        else
        {
            CEGUI::System &sys = CEGUI::System::getSingleton();
            sys.injectKeyDown(arg.key);
            sys.injectChar(arg.text);
        }

        return true;
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::keyReleased( const OIS::KeyEvent &arg )
    {
        CEGUI::System::getSingleton().injectKeyUp(arg.key);
        return true;
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::mouseMoved( const OIS::MouseEvent &arg )
    {
        const float ROTATE_MULTIPLIER = 0.2f;
        const float PITCH_MAX = 88.0f;
        const float PITCH_MIN = -PITCH_MAX;

        CEGUI::System &sys = CEGUI::System::getSingleton();

        bool mouseMoveHandled = sys.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
        // scroll wheel
        if (arg.state.Z.rel)
        {
            mouseMoveHandled = sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f) || mouseMoveHandled;
        }

        if (mIsExecutionSuspended)
        {
            return true;
        }

        if (!mouseMoveHandled && mMouse->getMouseState().buttonDown(OIS::MB_Right))
        {
            Ogre::Quaternion quat, quatTemp;

            float yawChange = -arg.state.X.rel * ROTATE_MULTIPLIER;
            mYaw += yawChange;
            Ogre::Radian yawRadian = (Ogre::Radian)Ogre::Math::DegreesToRadians(mYaw);

            float pitchChange = -arg.state.Y.rel * ROTATE_MULTIPLIER;
            mPitch = Ogre::Math::Clamp(mPitch + pitchChange, PITCH_MIN, PITCH_MAX);
            Ogre::Radian pitchRadian = (Ogre::Radian)Ogre::Math::DegreesToRadians(mPitch);

            mCamera->setOrientation(Ogre::Quaternion::IDENTITY);
            mCamera->yaw(yawRadian);
            mCamera->pitch(pitchRadian);
        }

        return true;
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
    {
        CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));

        // hide cursor when rotating camera
        if (id == OIS::MB_Right)
        {
            CEGUI::MouseCursor::getSingleton().hide();
            mSavedMousePosition = CEGUI::MouseCursor::getSingleton().getPosition();
        }

        return true;
    }

    //-------------------------------------------------------------------------------------
    bool BvhViewerApp::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
    {
        CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));

        // show cursor when finished rotating camera
        if (id == OIS::MB_Right)
        {
            CEGUI::MouseCursor::getSingleton().setPosition(mSavedMousePosition);
            CEGUI::MouseCursor::getSingleton().show();
        }

        return true;
    }
    /* END INPUT EVENT HANDLERS */

    //-------------------------------------------------------------------------------------
    CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
    {
        switch (buttonID)
        {
        case OIS::MB_Left:
            return CEGUI::LeftButton;
 
        case OIS::MB_Right:
            return CEGUI::RightButton;
 
        case OIS::MB_Middle:
            return CEGUI::MiddleButton;
 
        default:
            return CEGUI::LeftButton;
        }
    }

    //-------------------------------------------------------------------------------------
    void raiseException(string text)
    {
        stringstream ss;
        ss << text;
        throw new exception(ss.str().c_str());
    }
}
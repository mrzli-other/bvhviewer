#include "AnimationData.h"

namespace BVHViewer_Data
{
    using std::make_pair;

    AnimationData::AnimationData(BvhData *bvhData)
    {
        init(bvhData);
    }

    AnimationData::~AnimationData()
    {
        release();
    }

    void AnimationData::init(BvhData *bvhData)
    {
        mBvhData = bvhData;
        mFrameData = new FrameData[getFrameCount()];
    }

    void AnimationData::release()
    {
        if (mFrameData != NULL)
        {
            delete [] mFrameData;
            mFrameData = NULL;
        }

        delete mBvhData;
    }

    const Ogre::Vector3 &AnimationData::getTranslation(int nodeIndex, int frame)
    {
        BvhHierarchyNode bvhNode = mBvhData->getNode(nodeIndex);
        if (bvhNode.isRootNode())
        {
            return Ogre::Vector3::ZERO;
        }

        BvhHierarchyNode bvhParentNode = mBvhData->getNode(bvhNode.ParentIndex);
        if (bvhParentNode.ChannelCount != 6)
        {
            return Ogre::Vector3::ZERO;
        }

        typedef map<int, Ogre::Vector3>::const_iterator trans_it;

        FrameData &frameData = mFrameData[frame];
        trans_it itEnd = frameData.TranslationData.end();
        trans_it it = frameData.TranslationData.find(nodeIndex);
        if (it == itEnd)
        {
            float *motionData = mBvhData->getMotionData();
            int motionDataStride = mBvhData->getMotionDataStride();

            int firstDataIndex = frame * motionDataStride + bvhParentNode.FirstChannelIndex;

            Ogre::Vector3 translation(&motionData[firstDataIndex]);
            frameData.TranslationData.insert(make_pair(nodeIndex, translation));
            it = frameData.TranslationData.find(nodeIndex);
        }

        return it->second;
    }

    const Ogre::Quaternion &AnimationData::getRotation(int nodeIndex, int frame)
    {
        BvhHierarchyNode bvhNode = mBvhData->getNode(nodeIndex);
        if (bvhNode.isRootNode())
        {
            return Ogre::Quaternion::IDENTITY;
        }

        BvhHierarchyNode bvhParentNode = mBvhData->getNode(bvhNode.ParentIndex);

        typedef map<int, Ogre::Quaternion>::const_iterator rot_it;

        FrameData &frameData = mFrameData[frame];
        rot_it itEnd = frameData.RotationData.end();
        rot_it it = frameData.RotationData.find(nodeIndex);
        if (it == itEnd)
        {
            float *motionData = mBvhData->getMotionData();
            int motionDataStride = mBvhData->getMotionDataStride();

            int firstDataIndex = frame * motionDataStride + bvhParentNode.FirstChannelIndex;
            if (bvhParentNode.ChannelCount == 6)
            {
                firstDataIndex += 3;
            }

            Ogre::Radian angle;
            Ogre::Quaternion rot;

            angle = Ogre::Math::DegreesToRadians(motionData[firstDataIndex]);
            rot = Ogre::Quaternion(angle, Ogre::Vector3::UNIT_Z);
            angle = Ogre::Math::DegreesToRadians(motionData[firstDataIndex + 1]);
            rot = rot * Ogre::Quaternion(angle, Ogre::Vector3::UNIT_X);
            angle = Ogre::Math::DegreesToRadians(motionData[firstDataIndex + 2]);
            rot = rot * Ogre::Quaternion(angle, Ogre::Vector3::UNIT_Y);

            Ogre::Quaternion parentRot = getRotation(bvhParentNode.NodeIndex, frame);
            rot = parentRot * rot;

            frameData.RotationData.insert(make_pair(nodeIndex, rot));
            it = frameData.RotationData.find(nodeIndex);
        }

        return it->second;
    }
}
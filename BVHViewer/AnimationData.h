#ifndef ANIMATION_DATA_H
#define ANIMATION_DATA_H

#include "BvhData.h"
#include "FrameData.h"

namespace BVHViewer_Data
{
    using namespace BVHViewer_DataObjects;

    class AnimationData
    {
    // ===========================================================
    // Constants
    // ===========================================================

    // ===========================================================
    // Fields
    // ===========================================================
    private:
        const BvhData *mBvhData;

        FrameData *mFrameData;

    // ===========================================================
    // Constructors/Destructors
    // ===========================================================
    public:
        AnimationData(BvhData *bvhData);
        ~AnimationData();

    private:
        void init(BvhData *bvhData);
        void release();

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================

    // ===========================================================
    // Methods
    // ===========================================================
    public:
        const Ogre::Quaternion &getRotation(int nodeIndex, int frame);
        const Ogre::Vector3 &getTranslation(int nodeIndex, int frame);

    // ===========================================================
    // Inner and Anonymous Classes
    // ===========================================================
    
    // ===========================================================
    // Getter & Setter
    // ===========================================================
    public:
        int getFrameCount() const { return mBvhData->getFrameCount() >= 0 ? mBvhData->getFrameCount() : 0; } 
        float getFrameTime() const { return mBvhData->getFrameTime(); }
    };
}

#endif
#ifndef BVH_DATA_H
#define BVH_DATA_H

#include <vector>
#include "BvhHierarchyNode.h"

namespace BVHViewer_DataObjects
{
    using std::vector;

    class BvhData
    {
    // ===========================================================
    // Constants
    // ===========================================================

    // ===========================================================
    // Fields
    // ===========================================================
    private:
        vector<BvhHierarchyNode> mBvhNodes;
        float *mMotionData;
        int mMotionDataStride;
        int mFrameCount;
        float mFrameTime;

    // ===========================================================
    // Constructors/Destructors
    // ===========================================================
    public:
        __declspec(dllexport) BvhData();
        __declspec(dllexport) ~BvhData();

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================

    // ===========================================================
    // Methods
    // ===========================================================
    private:
        void release();

    // ===========================================================
    // Inner and Anonymous Classes
    // ===========================================================
    
    // ===========================================================
    // Getter & Setter
    // ===========================================================
    public:
        vector<BvhHierarchyNode> __declspec(dllexport) getRootNodes() const;
        vector<BvhHierarchyNode> __declspec(dllexport) getChildNodes(int parentIndex) const;
        BvhHierarchyNode __declspec(dllexport) getNode(int nodeIndex) const;
        int __declspec(dllexport) getMaxParentIndex() const;
        void  setBvhNodes(const vector<BvhHierarchyNode> &bvhNodes) { mBvhNodes = bvhNodes; }

        float __declspec(dllexport) *getMotionData() const { return mMotionData; }
        void __declspec(dllexport) setMotionData(float *motionData) { mMotionData = motionData; }

        int __declspec(dllexport) getMotionDataStride() const { return mMotionDataStride; }
        void __declspec(dllexport) setMotionDataStride(int motionDataStride) { mMotionDataStride = motionDataStride; }

        int __declspec(dllexport) getMotionDataSize() const { return mMotionDataStride * mFrameCount; }

        int __declspec(dllexport) getFrameCount() const { return mFrameCount; }
        void __declspec(dllexport) setFrameCount(int frameCount) { mFrameCount = frameCount; }

        float __declspec(dllexport) getFrameTime() const { return mFrameTime; }
        void __declspec(dllexport) setFrameTime(float frameTime) { mFrameTime = frameTime; }
    };
}

#endif
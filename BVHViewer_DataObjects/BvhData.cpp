#include "BvhData.h"

namespace BVHViewer_DataObjects
{
    using std::exception;

    BvhData::BvhData()
    {
    }

    BvhData::~BvhData()
    {
        release();
    }

    void BvhData::release()
    {
        if (mMotionData != NULL)
        {
            delete [] mMotionData;
            mMotionData = NULL;
        }
    }

    vector<BvhHierarchyNode> BvhData::getRootNodes() const
    {
        vector<BvhHierarchyNode> childNodes;
        int size = mBvhNodes.size();
        for (int i = 0; i < size; i++)
        {
            BvhHierarchyNode bvhNode = mBvhNodes[i];
            if (bvhNode.isRootNode())
            {
                childNodes.push_back(bvhNode);
            }
        }

        return childNodes;
    }

    vector<BvhHierarchyNode> BvhData::getChildNodes(int parentIndex) const
    {
        vector<BvhHierarchyNode> childNodes;
        int size = mBvhNodes.size();
        for (int i = 0; i < size; i++)
        {
            BvhHierarchyNode bvhNode = mBvhNodes[i];
            if (bvhNode.ParentIndex == parentIndex)
            {
                childNodes.push_back(bvhNode);
            }
        }

        return childNodes;
    }

    BvhHierarchyNode BvhData::getNode(int nodeIndex) const
    {
        vector<BvhHierarchyNode> childNodes;
        int size = mBvhNodes.size();
        for (int i = 0; i < size; i++)
        {
            BvhHierarchyNode bvhNode = mBvhNodes[i];
            if (bvhNode.NodeIndex == nodeIndex)
            {
                return bvhNode;
            }
        }

        throw new exception("Invalid node index.");
    }

    int BvhData::getMaxParentIndex() const
    {
        int maxParentIndex = -1;
        int size = mBvhNodes.size();
        for (int i = 0; i < size; i++)
        {
            int parentIndex = mBvhNodes[i].ParentIndex;
            maxParentIndex = parentIndex > maxParentIndex ? parentIndex : maxParentIndex;
        }

        return maxParentIndex;
    }
}
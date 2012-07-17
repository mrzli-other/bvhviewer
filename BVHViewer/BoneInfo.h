#ifndef BONE_INFO_H
#define BONE_INFO_H

#include <OgreRoot.h>

namespace BVHViewer_Data
{
    struct BoneInfo
    {
        unsigned long ParentIndex;
        float Length;
        Ogre::SceneNode *SceneNode;
        unsigned long SkeletonHandle;
        unsigned long BoneHandle;
    };
}

#endif
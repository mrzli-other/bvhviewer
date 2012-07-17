#ifndef FRAME_DATA_H
#define FRAME_DATA_H

#include <map>

#include <OgreRoot.h>

namespace BVHViewer_Data
{
    using std::map;

    struct FrameData
    {
        map<int, Ogre::Quaternion> RotationData;
        map<int, Ogre::Vector3> TranslationData;
    };
}

#endif
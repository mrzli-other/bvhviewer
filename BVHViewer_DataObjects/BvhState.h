#ifndef BVH_STATE_H
#define BVH_STATE_H

namespace BVHViewer_DataObjects
{
    /*
    Finite state machine state which determine the lexical unit.
    */
    enum BvhState
    {
        E_BVH_STATE_BEGIN,
        E_BVH_STATE_UNDERSCORE,
        E_BVH_STATE_IDENTIFIER,
        E_BVH_STATE_SIGN,
        E_BVH_STATE_INTEGER,
        E_BVH_STATE_FLOAT,
        E_BVH_STATE_FAIL,
        E_BVH_STATES_COUNT
    };
}

#endif
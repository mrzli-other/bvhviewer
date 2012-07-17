#ifndef BVH_CHAR_TYPE_H
#define BVH_CHAR_TYPE_H

namespace BVHViewer_DataObjects
{
    /*
    Bvh Character type.
    */
    enum BvhCharType
    {
        E_BVH_CHAR_TYPE_LETTER,		// letter
        E_BVH_CHAR_TYPE_DIGIT,			// digit
        E_BVH_CHAR_TYPE_SPECIALSIGN,	// special sign, '{', '}' ili ':'
        E_BVH_CHAR_TYPE_BLANKSIGN,		// space, ' ', '\t' ili '\n'
        E_BVH_CHAR_TYPE_UNDERSCORE,	// underscore '_'
        E_BVH_CHAR_TYPE_SIGN,			// sign '-' ili '+'
        E_BVH_CHAR_TYPE_POINT,			// point '.'
        E_BVH_CHAR_TYPE_ERROR,			// invalid sign
        E_BVH_CHAR_TYPES_COUNT
    };
}

#endif
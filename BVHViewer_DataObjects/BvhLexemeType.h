#ifndef BVH_LEXEME_TYPE_H
#define BVH_LEXEME_TYPE_H

namespace BVHViewer_DataObjects
{
    /*
    Bvh Lexeme Type.
    */
    enum BvhLexemeType
    {
        E_BVH_LEXEME_TYPE_KEYWORD,
        E_BVH_LEXEME_TYPE_SPECIALSIGN,
        E_BVH_LEXEME_TYPE_IDENTIFIER,
        E_BVH_LEXEME_TYPE_INTEGER,
        E_BVH_LEXEME_TYPE_FLOAT,
        E_BVH_LEXEME_TYPE_ENDSIGN
    };
}

#endif
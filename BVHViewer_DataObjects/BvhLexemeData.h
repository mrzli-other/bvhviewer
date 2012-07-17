#ifndef BVH_LEXEME_DATA_H
#define BVH_LEXEME_DATA_H

#include <string>
#include "BvhLexemeType.h"
#include "BvhToken.h"

namespace BVHViewer_DataObjects
{
    using std::string;

    /*
    Lexeme data.
    */
    struct __declspec(dllexport) BvhLexemeData
    {
        BvhLexemeType LexemeType; // lexeme type
        int LineNumber; // line number fo lexeme (in canse of synta analysis error)
        void *Value; // lexeme value

        BvhToken getKeywordValue() const { return *(static_cast<BvhToken *>(Value)); }
        string getIdentifierValue() const { return *(static_cast<string *>(Value)); }
        int getIntValue() const { return *(static_cast<int *>(Value)); }
        float getFloatValue() const { return *(static_cast<float *>(Value)); }
    };
}

#endif
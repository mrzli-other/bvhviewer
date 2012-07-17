#ifndef BVH_LEXING_DATA_H
#define BVH_LEXING_DATA_H

#include <vector>
#include <set>
#include <string>
#include "BvhLexemeData.h"

namespace BVHViewer_DataObjects
{
    using std::vector;

    class BvhLexingData
    {
    // ===========================================================
    // Constants
    // ===========================================================

    // ===========================================================
    // Fields
    // ===========================================================
    private:
        vector<BvhLexemeData> mLexemes;

    // ===========================================================
    // Constructors/Destructors
    // ===========================================================
    public:
        __declspec(dllexport) BvhLexingData();
        __declspec(dllexport) ~BvhLexingData();

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================

    // ===========================================================
    // Methods
    // ===========================================================
    public:
        void __declspec(dllexport) addLexeme(BvhLexemeData &lexemeData) { mLexemes.push_back(lexemeData); }

    private:
        void release();

    // ===========================================================
    // Inner and Anonymous Classes
    // ===========================================================
    
    // ===========================================================
    // Getter & Setter
    // ===========================================================
    public:
        const vector<BvhLexemeData> __declspec(dllexport) &getLexemes() const { return mLexemes; }
    };
}

#endif
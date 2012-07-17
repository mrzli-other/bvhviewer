#ifndef BVH_LEXER_H
#define BVH_LEXER_H

#include <string>
#include <vector>
#include "BvhState.h"
#include "BvhCharType.h"
#include "BvhLexemeData.h"
#include "BvhLexingData.h"

namespace BVHViewer_DataAccess
{
    using namespace BVHViewer_DataObjects;
    using std::string;
    using std::vector;

    class BvhLexer
    {
        friend class BvhLexerStaticInitializer;

    // ===========================================================
    // Constants
    // ===========================================================

    // ===========================================================
    // Fields
    // ===========================================================
    private:
        static BvhState mTransitionTable[E_BVH_STATES_COUNT][E_BVH_CHAR_TYPES_COUNT];

    // ===========================================================
    // Constructors/Destructors
    // ===========================================================
    private:
        BvhLexer();

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================

    // ===========================================================
    // Methods
    // ===========================================================
    public:
        static void run(BvhLexingData *lexingData, const string &bvhStr);

    private:
        static BvhCharType getCharType(const char c);

        static void handleStateBegin(
            BvhLexingData *bvhLexingData,
            const BvhState currState,
            const string lexemeStr,
            const char ch,
            const int lineNum);

        static void raiseException(const int lineNum);

    // ===========================================================
    // Inner and Anonymous Classes
    // ===========================================================
    
    // ===========================================================
    // Getter & Setter
    // ===========================================================
    };

    class BvhLexerStaticInitializer
    {
    private:
        static BvhLexer mBvhLexer;
    };
}

#endif
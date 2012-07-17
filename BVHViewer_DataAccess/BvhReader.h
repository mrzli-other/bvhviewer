#ifndef BVH_READER_H
#define BVH_READER_H

#include <map>
#include <string>
#include "BvhLexingData.h"
#include "BvhData.h"
#include "BvhToken.h"

namespace BVHViewer_DataAccess
{
    using namespace BVHViewer_DataObjects;
    using std::map;
    using std::string;

    class BvhReader
    {
    // ===========================================================
    // Constants
    // ===========================================================

    // ===========================================================
    // Fields
    // ===========================================================
    private:
        static map<string, BvhToken> mKeywordValues;
        static map<BvhToken, string> mTokenRepresentations;

        static string mEndSiteNodeName;

    // ===========================================================
    // Constructors/Destructors
    // ===========================================================
    public:
        BvhReader();

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================

    // ===========================================================
    // Methods
    // ===========================================================
    public:
        static void __declspec(dllexport) read(BvhData *bvhData, const string &fileName);
        static bool isKeyword(const string &str);
        static BvhToken getKeywordValue(const string &str);

    private:
        static void disposeLexingData(const BvhLexingData &lexingData);

        static string getDebugInfo(const BvhLexingData *lexingData);
        static string getDebugInfo(const BvhLexemeData &lexemeData);
        static string getLexemeTypeString(const BvhLexemeType lexemeType);
        static string getLexemeValueString(const BvhLexemeData &lexemeData);
        static string getTokenRepresentation(const BvhToken bvhToken);

    // ===========================================================
    // Inner and Anonymous Classes
    // ===========================================================
    
    // ===========================================================
    // Getter & Setter
    // ===========================================================
    public:
        static string getEndSiteNodeName();
    };

    class BvhReaderStaticInitializer
    {
    private:
        static BvhReader mBvhReader;
    };
}

#endif
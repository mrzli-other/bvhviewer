#ifndef BVH_PARSER_H
#define BVH_PARSER_H

#include <stack>
#include <vector>
#include "BvhData.h"
#include "BvhLexingData.h"
#include "BvhHierarchyNode.h"

namespace BVHViewer_DataAccess
{
    using namespace BVHViewer_DataObjects;
    using std::stack;
    using std::vector;

    class BvhParser
    {
    // ===========================================================
    // Constants
    // ===========================================================

    // ===========================================================
    // Fields
    // ===========================================================

    // ===========================================================
    // Constructors/Destructors
    // ===========================================================
    private:
        BvhParser();

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================

    // ===========================================================
    // Methods
    // ===========================================================
    public:
        static void run(BvhData *bvhData, const BvhLexingData *lexingData);

    private:
        static int getNumberOfChannels(const vector<BvhHierarchyNode> &bvhHierarchyNodeList);
        static int getParentIndex(const stack<BvhHierarchyNode> &bvhHierarchyNodeStack);

        static void raiseException(const int lineNum);

    // ===========================================================
    // Inner and Anonymous Classes
    // ===========================================================
    
    // ===========================================================
    // Getter & Setter
    // ===========================================================
    };
}

#endif
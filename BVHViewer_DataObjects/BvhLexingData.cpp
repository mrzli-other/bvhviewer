#include "BvhLexingData.h"

namespace BVHViewer_DataObjects
{
    using std::exception;

    BvhLexingData::BvhLexingData()
    {
    }

    BvhLexingData::~BvhLexingData()
    {
        release();
    }

    void BvhLexingData::release()
    {
        int size = mLexemes.size();
        for (int i = 0; i < size; i++)
        {
            BvhLexemeData lexeme = mLexemes[i];
            if (lexeme.Value != NULL)
            {
                delete lexeme.Value;
            }
        }
    }
}
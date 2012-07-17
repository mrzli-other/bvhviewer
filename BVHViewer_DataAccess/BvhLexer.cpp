#include <exception>
#include <sstream>
#include "BvhLexer.h"
#include "BvhReader.h"

namespace BVHViewer_DataAccess
{
    using namespace BVHViewer_DataObjects;
    using std::vector;
    using std::string;
    using std::stringstream;
    using std::exception;
    using std::endl;

    BvhState BvhLexer::mTransitionTable[E_BVH_STATES_COUNT][E_BVH_CHAR_TYPES_COUNT];
    BvhLexer BvhLexerStaticInitializer::mBvhLexer;

    BvhLexer::BvhLexer()
    {
        mTransitionTable[E_BVH_STATE_BEGIN][E_BVH_CHAR_TYPE_LETTER] = E_BVH_STATE_IDENTIFIER;
        mTransitionTable[E_BVH_STATE_BEGIN][E_BVH_CHAR_TYPE_DIGIT] = E_BVH_STATE_INTEGER;
        mTransitionTable[E_BVH_STATE_BEGIN][E_BVH_CHAR_TYPE_SPECIALSIGN] = E_BVH_STATE_BEGIN;
        mTransitionTable[E_BVH_STATE_BEGIN][E_BVH_CHAR_TYPE_BLANKSIGN] = E_BVH_STATE_BEGIN;
        mTransitionTable[E_BVH_STATE_BEGIN][E_BVH_CHAR_TYPE_UNDERSCORE] = E_BVH_STATE_UNDERSCORE;
        mTransitionTable[E_BVH_STATE_BEGIN][E_BVH_CHAR_TYPE_SIGN] = E_BVH_STATE_SIGN;
        mTransitionTable[E_BVH_STATE_BEGIN][E_BVH_CHAR_TYPE_POINT] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_BEGIN][E_BVH_CHAR_TYPE_ERROR] = E_BVH_STATE_FAIL;

        mTransitionTable[E_BVH_STATE_UNDERSCORE][E_BVH_CHAR_TYPE_LETTER] = E_BVH_STATE_IDENTIFIER;
        mTransitionTable[E_BVH_STATE_UNDERSCORE][E_BVH_CHAR_TYPE_DIGIT] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_UNDERSCORE][E_BVH_CHAR_TYPE_SPECIALSIGN] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_UNDERSCORE][E_BVH_CHAR_TYPE_BLANKSIGN] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_UNDERSCORE][E_BVH_CHAR_TYPE_UNDERSCORE] = E_BVH_STATE_UNDERSCORE;
        mTransitionTable[E_BVH_STATE_UNDERSCORE][E_BVH_CHAR_TYPE_SIGN] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_UNDERSCORE][E_BVH_CHAR_TYPE_POINT] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_UNDERSCORE][E_BVH_CHAR_TYPE_ERROR] = E_BVH_STATE_FAIL;

        mTransitionTable[E_BVH_STATE_IDENTIFIER][E_BVH_CHAR_TYPE_LETTER] = E_BVH_STATE_IDENTIFIER;
        mTransitionTable[E_BVH_STATE_IDENTIFIER][E_BVH_CHAR_TYPE_DIGIT] = E_BVH_STATE_IDENTIFIER;
        mTransitionTable[E_BVH_STATE_IDENTIFIER][E_BVH_CHAR_TYPE_SPECIALSIGN] = E_BVH_STATE_BEGIN;
        mTransitionTable[E_BVH_STATE_IDENTIFIER][E_BVH_CHAR_TYPE_BLANKSIGN] = E_BVH_STATE_BEGIN;
        mTransitionTable[E_BVH_STATE_IDENTIFIER][E_BVH_CHAR_TYPE_UNDERSCORE] = E_BVH_STATE_IDENTIFIER;
        mTransitionTable[E_BVH_STATE_IDENTIFIER][E_BVH_CHAR_TYPE_SIGN] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_IDENTIFIER][E_BVH_CHAR_TYPE_POINT] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_IDENTIFIER][E_BVH_CHAR_TYPE_ERROR] = E_BVH_STATE_FAIL;

        mTransitionTable[E_BVH_STATE_SIGN][E_BVH_CHAR_TYPE_LETTER] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_SIGN][E_BVH_CHAR_TYPE_DIGIT] = E_BVH_STATE_INTEGER;
        mTransitionTable[E_BVH_STATE_SIGN][E_BVH_CHAR_TYPE_SPECIALSIGN] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_SIGN][E_BVH_CHAR_TYPE_BLANKSIGN] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_SIGN][E_BVH_CHAR_TYPE_UNDERSCORE] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_SIGN][E_BVH_CHAR_TYPE_SIGN] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_SIGN][E_BVH_CHAR_TYPE_POINT] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_SIGN][E_BVH_CHAR_TYPE_ERROR] = E_BVH_STATE_FAIL;

        mTransitionTable[E_BVH_STATE_INTEGER][E_BVH_CHAR_TYPE_LETTER] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_INTEGER][E_BVH_CHAR_TYPE_DIGIT] = E_BVH_STATE_INTEGER;
        mTransitionTable[E_BVH_STATE_INTEGER][E_BVH_CHAR_TYPE_SPECIALSIGN] = E_BVH_STATE_BEGIN;
        mTransitionTable[E_BVH_STATE_INTEGER][E_BVH_CHAR_TYPE_BLANKSIGN] = E_BVH_STATE_BEGIN;
        mTransitionTable[E_BVH_STATE_INTEGER][E_BVH_CHAR_TYPE_UNDERSCORE] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_INTEGER][E_BVH_CHAR_TYPE_SIGN] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_INTEGER][E_BVH_CHAR_TYPE_POINT] = E_BVH_STATE_FLOAT;
        mTransitionTable[E_BVH_STATE_INTEGER][E_BVH_CHAR_TYPE_ERROR] = E_BVH_STATE_FAIL;

        mTransitionTable[E_BVH_STATE_FLOAT][E_BVH_CHAR_TYPE_LETTER] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_FLOAT][E_BVH_CHAR_TYPE_DIGIT] = E_BVH_STATE_FLOAT;
        mTransitionTable[E_BVH_STATE_FLOAT][E_BVH_CHAR_TYPE_SPECIALSIGN] = E_BVH_STATE_BEGIN;
        mTransitionTable[E_BVH_STATE_FLOAT][E_BVH_CHAR_TYPE_BLANKSIGN] = E_BVH_STATE_BEGIN;
        mTransitionTable[E_BVH_STATE_FLOAT][E_BVH_CHAR_TYPE_UNDERSCORE] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_FLOAT][E_BVH_CHAR_TYPE_SIGN] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_FLOAT][E_BVH_CHAR_TYPE_POINT] = E_BVH_STATE_FAIL;
        mTransitionTable[E_BVH_STATE_FLOAT][E_BVH_CHAR_TYPE_ERROR] = E_BVH_STATE_FAIL;
    }

    void BvhLexer::run(BvhLexingData *lexingData, const string &bvhStr)
    {
        BvhState currState = E_BVH_STATE_BEGIN;

        int lineNum = 1;
        string lexemeStr = "";

        int bvhStrLength = static_cast<int>(bvhStr.length());
        for (int i = 0; i < bvhStrLength; i++)
        {
            const char ch = bvhStr[i];

            BvhCharType charType = getCharType(ch);
            BvhState newState = mTransitionTable[currState][charType];
            if (newState == E_BVH_STATE_FAIL)
            {
                raiseException(lineNum);
            }

            switch (newState)
            {
            case E_BVH_STATE_BEGIN:
                handleStateBegin(
                    lexingData,
                    currState,
                    lexemeStr,
                    ch,
                    lineNum);
                lexemeStr = "";
                break;

            case E_BVH_STATE_UNDERSCORE:
            case E_BVH_STATE_IDENTIFIER:
            case E_BVH_STATE_SIGN:
            case E_BVH_STATE_INTEGER:
            case E_BVH_STATE_FLOAT:
                lexemeStr += ch;
                break;

            case E_BVH_STATE_FAIL:
            default:
                raiseException(lineNum);
            }

            if (ch == '\n') {
                ++lineNum;
            }

            currState = newState;
        }

        BvhLexemeData endSignLexeme = { E_BVH_LEXEME_TYPE_ENDSIGN, lineNum, NULL };
        lexingData->addLexeme(endSignLexeme);
    }

    void BvhLexer::handleStateBegin(
        BvhLexingData *lexingData,
        const BvhState currState,
        const string lexemeStr,
        const char ch,
        const int lineNum)
    {
        switch (currState)
        {
        case E_BVH_STATE_BEGIN:
            break;

        case E_BVH_STATE_IDENTIFIER:
            if (BvhReader::isKeyword(lexemeStr))
            {
                BvhToken keywordValue = BvhReader::getKeywordValue(lexemeStr);
                BvhLexemeData lexeme = { E_BVH_LEXEME_TYPE_KEYWORD, lineNum, new BvhToken(keywordValue) };
                lexingData->addLexeme(lexeme);
            }
            else
            {
                BvhLexemeData lexeme = { E_BVH_LEXEME_TYPE_IDENTIFIER, lineNum, new string(lexemeStr) };
                lexingData->addLexeme(lexeme);
            }
            break;
            
        case E_BVH_STATE_INTEGER:
            {
                BvhLexemeData lexeme = { E_BVH_LEXEME_TYPE_INTEGER, lineNum, new int(atoi(lexemeStr.c_str())) };
                lexingData->addLexeme(lexeme);
            }
            break;

        case E_BVH_STATE_FLOAT:
            {
                BvhLexemeData lexeme = { E_BVH_LEXEME_TYPE_FLOAT, lineNum, new float(static_cast<float>(atof(lexemeStr.c_str()))) };
                lexingData->addLexeme(lexeme);
            }
            break;

        case E_BVH_STATE_FAIL:
        case E_BVH_STATE_UNDERSCORE:
        case E_BVH_STATE_SIGN:
        default:
            raiseException(lineNum);
        }

        // if E_BVH_CHAR_TYPE_SPECIALSIGN was read add it to list of lexemes
        if (BvhLexer::getCharType(ch) == E_BVH_CHAR_TYPE_SPECIALSIGN)
        {
            stringstream ss;
            ss << ch;
            BvhToken keywordValue = BvhReader::getKeywordValue(ss.str());
            BvhLexemeData lexeme = { E_BVH_LEXEME_TYPE_SPECIALSIGN, lineNum, new BvhToken(keywordValue) };
            lexingData->addLexeme(lexeme);
        }
    }

    BvhCharType BvhLexer::getCharType(const char c)
    {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        {
            return E_BVH_CHAR_TYPE_LETTER;
        }
        else if (c >= '0' && c <= '9' )
        {
            return E_BVH_CHAR_TYPE_DIGIT;
        }
        else if (c == '{' || c == '}' || c == ':')
        {
            return E_BVH_CHAR_TYPE_SPECIALSIGN;
        }
        else if (c == ' ' || c == '\t' || c == '\n')
        {
            return E_BVH_CHAR_TYPE_BLANKSIGN;
        }
        else if (c == '_')
        {
            return E_BVH_CHAR_TYPE_UNDERSCORE;
        }
        else if (c == '+' || c == '-')
        {
            return E_BVH_CHAR_TYPE_SIGN;
        }
        else if (c == '.')
        {
            return E_BVH_CHAR_TYPE_POINT;
        }
        else
        {
            return E_BVH_CHAR_TYPE_ERROR;
        }
    }

    void BvhLexer::raiseException(const int lineNum)
    {
        stringstream ss;
        ss << "Lexing error at line " << lineNum << ".";
        throw new exception(ss.str().c_str());
    }
}
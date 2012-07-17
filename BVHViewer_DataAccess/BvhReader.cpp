#include <fstream>
#include <sstream>
#include <exception>
#include <iomanip>
#include <vector>
#include "BvhReader.h"
#include "BvhLexer.h"
#include "BvhParser.h"

namespace BVHViewer_DataAccess
{
    using namespace BVHViewer_DataObjects;
    using std::map;
    using std::make_pair;
    using std::string;
    using std::basic_ifstream;
    using std::istreambuf_iterator;
    using std::stringstream;
    using std::exception;
    using std::vector;

    map<string, BvhToken> BvhReader::mKeywordValues;
    map<BvhToken, string> BvhReader::mTokenRepresentations;
    string BvhReader::mEndSiteNodeName = "End_Site";
    BvhReader BvhReaderStaticInitializer::mBvhReader;

    BvhReader::BvhReader()
    {
        if (mKeywordValues.empty())
        {
            mKeywordValues.insert(make_pair("HIERARCHY", E_BVH_TOKEN_TKW_HIERARCHY));
            mKeywordValues.insert(make_pair("ROOT", E_BVH_TOKEN_TKW_ROOT));
            mKeywordValues.insert(make_pair("JOINT", E_BVH_TOKEN_TKW_JOINT));
            mKeywordValues.insert(make_pair("OFFSET", E_BVH_TOKEN_TKW_OFFSET));
            mKeywordValues.insert(make_pair("CHANNELS", E_BVH_TOKEN_TKW_CHANNELS));
            mKeywordValues.insert(make_pair("Xposition", E_BVH_TOKEN_TKW_XPOSITION));
            mKeywordValues.insert(make_pair("Yposition", E_BVH_TOKEN_TKW_YPOSITION));
            mKeywordValues.insert(make_pair("Zposition", E_BVH_TOKEN_TKW_ZPOSITION));
            mKeywordValues.insert(make_pair("Xrotation", E_BVH_TOKEN_TKW_XROTATION));
            mKeywordValues.insert(make_pair("Yrotation", E_BVH_TOKEN_TKW_YROTATION));
            mKeywordValues.insert(make_pair("Zrotation", E_BVH_TOKEN_TKW_ZROTATION));
            mKeywordValues.insert(make_pair("End", E_BVH_TOKEN_TKW_END));
            mKeywordValues.insert(make_pair("Site", E_BVH_TOKEN_TKW_SITE));
            mKeywordValues.insert(make_pair("MOTION", E_BVH_TOKEN_TKW_MOTION));
            mKeywordValues.insert(make_pair("Frames", E_BVH_TOKEN_TKW_FRAMES));
            mKeywordValues.insert(make_pair("Frame", E_BVH_TOKEN_TKW_FRAME));
            mKeywordValues.insert(make_pair("Time", E_BVH_TOKEN_TKW_TIME));
            mKeywordValues.insert(make_pair("{", E_BVH_TOKEN_TKW_OPEN_BRACKET));
            mKeywordValues.insert(make_pair("}", E_BVH_TOKEN_TKW_CLOSE_BRACKET));
            mKeywordValues.insert(make_pair(":", E_BVH_TOKEN_TKW_COLON));
        }

        if (mTokenRepresentations.empty())
        {
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_NT_BOTTOM, "<__BOTTOM__>"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_NT_FILE, "<FILE>"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_NT_HIERARCHY, "<HIERARCHY>"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_NT_ROOT_LIST, "<ROOT_LIST>"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_NT_ROOT, "<ROOT>"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_NT_ROOT_LIST_CONTINUE, "<ROOT_LIST_CONTINUE>"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_NT_OFFSET, "<OFFSET>"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_NT_CHANNELS, "<CHANNELS>"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_NT_KEYWORD_POSITION_LIST, "<KEYWORD_POSITION_LIST>"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_NT_KEYWORD_POSITION, "<KEYWORD_POSITION>"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_NT_JOINT_LIST, "<JOINT_LIST>"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_NT_JOINT_LIST_CONTINUE, "<JOINT_LIST_CONTINUE>"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_NT_MOTION, "<MOTION>"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_NT_MOTION_FLOAT_LIST, "<MOTION_FLOAT_LIST>"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_HIERARCHY, "HIERARCHY"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_ROOT, "ROOT"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_JOINT, "JOINT"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_OFFSET, "OFFSET"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_CHANNELS, "CHANNELS"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_XPOSITION, "Xposition"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_YPOSITION, "Yposition"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_ZPOSITION, "Zposition"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_XROTATION, "Xrotation"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_YROTATION, "Yrotation"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_ZROTATION, "Zrotation"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_END, "End"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_SITE, "Site"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_MOTION, "MOTION"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_FRAMES, "Frames"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_FRAME, "Frame"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_TIME, "Time"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_OPEN_BRACKET, "{"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_CLOSE_BRACKET, "}"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_TKW_COLON, ":"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_T_IDENTIFIER, "identifier"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_T_INTEGER_CHANNELS, "integer_channels"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_T_INTEGER_FRAMES, "integer_frames"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_T_FLOAT_OFFSET, "float_offset"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_T_FLOAT_FRAMETIME, "float_frametime"));
            mTokenRepresentations.insert(make_pair(E_BVH_TOKEN_T_FLOAT_MOTIONDATA, "float_motiondata"));
        }
    }

    void BvhReader::read(BvhData *bvhData, const string &bvhFilePath)
    {
        basic_ifstream<char> bvhFile(bvhFilePath);

        if (!bvhFile.is_open())
        {
            stringstream ss;
            ss << "Failed to open file:  '" << bvhFilePath << "'.";
            throw new exception(ss.str().c_str());
        }

        string bvhStr(
            (istreambuf_iterator<char>(bvhFile)),
            istreambuf_iterator<char>());

        BvhLexingData *lexingData = new BvhLexingData();

        BvhLexer::run(lexingData, bvhStr);
        BvhParser::run(bvhData, lexingData);

        //string debugStr = getDebugInfo(lexingData);

        delete lexingData;

        bvhFile.close();
    }

    bool BvhReader::isKeyword(const string &str)
    {
        return mKeywordValues.find(str) != mKeywordValues.end();
    }

    BvhToken BvhReader::getKeywordValue(const string &str)
    {
        return mKeywordValues[str];
    }

    string BvhReader::getDebugInfo(const BvhLexingData *lexingData)
    {
        using std::endl;

        stringstream ss;

        vector<BvhLexemeData> lexemes = lexingData->getLexemes();
        int size = lexemes.size();
        for (int i = 0; i < size; i++)
        {
            ss << getDebugInfo(lexemes[i]) << endl;
        }

        return ss.str();
    }

    string BvhReader::getDebugInfo(const BvhLexemeData &lexemeData)
    {
        using std::left;
        using std::right;
        using std::setw;
        using std::endl;

        stringstream ss;
        ss <<
            "Type: " << left << setw(12) << getLexemeTypeString(lexemeData.LexemeType) <<
            " | Line: " << right << setw(5) << lexemeData.LineNumber <<
            " | Value: " << left << setw(20) << getLexemeValueString(lexemeData);

        return ss.str();
    }

    string BvhReader::getLexemeTypeString(const BvhLexemeType lexemeType)
    {
        switch (lexemeType)
        {
        case E_BVH_LEXEME_TYPE_KEYWORD:
            return "Keyword";
        case E_BVH_LEXEME_TYPE_SPECIALSIGN:
            return "SpecialSign";
        case E_BVH_LEXEME_TYPE_IDENTIFIER:
            return "Identifier";
        case E_BVH_LEXEME_TYPE_INTEGER:
            return "Integer";
        case E_BVH_LEXEME_TYPE_FLOAT:
            return "Float";
        case E_BVH_LEXEME_TYPE_ENDSIGN:
            return "EndSign";
        default:
            return "Unknown";
        }
    }

    string BvhReader::getLexemeValueString(const BvhLexemeData &lexemeData)
    {
        stringstream ss;
        switch (lexemeData.LexemeType)
        {
        case E_BVH_LEXEME_TYPE_KEYWORD:
        case E_BVH_LEXEME_TYPE_SPECIALSIGN:
            return getTokenRepresentation(lexemeData.getKeywordValue());
        case E_BVH_LEXEME_TYPE_IDENTIFIER:
            return lexemeData.getIdentifierValue();
        case E_BVH_LEXEME_TYPE_INTEGER:
            ss << lexemeData.getIntValue();
            return ss.str();
        case E_BVH_LEXEME_TYPE_FLOAT:
            ss << lexemeData.getFloatValue();
            return ss.str();
        case E_BVH_LEXEME_TYPE_ENDSIGN:
            return "None";
        default:
            return "Unknown";
        }
    }

    string BvhReader::getTokenRepresentation(const BvhToken bvhToken)
    {
        return mTokenRepresentations[bvhToken];
    }

    string BvhReader::getEndSiteNodeName()
    {
        return mEndSiteNodeName;
    }
}
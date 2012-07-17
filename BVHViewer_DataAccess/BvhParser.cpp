#include <sstream>
#include <exception>
#include "BvhParser.h"
#include "BvhReader.h"

namespace BVHViewer_DataAccess
{
    using namespace BVHViewer_DataObjects;
    using std::string;
    using std::stringstream;
    using std::exception;
    using std::stack;
    using std::vector;

    BvhParser::BvhParser()
    {
    }

    void BvhParser::run(BvhData *bvhData, const BvhLexingData *lexingData)
    {
        stack<BvhToken> parsingStack;

        // set initial stack
        parsingStack.push(E_BVH_TOKEN_NT_BOTTOM);
        parsingStack.push(E_BVH_TOKEN_NT_FILE);

        stack<BvhHierarchyNode> bvhHierarchyNodeStack;
        vector<BvhHierarchyNode> bvhHierarchyNodeList;

        int currSkeletonIndex = -1;
        int currNodeIndex = 1;
        int currOffsetFilledIndex = 0;

        int channelsNum = -1;
        int framesNum = -1;
        int motionDataSize = -1;
        float frameLength = 0.0f;
        int currMotionDataFilledIndex = 0;

        // index of first channel in motion data for current node
        int currFirstChannelIndex = 0;

        float *motionData = NULL;

        vector<BvhLexemeData> lexemes = lexingData->getLexemes();
        int index = 0;
        int lexemesSize = lexemes.size();
        while (index < lexemesSize)
        {
            BvhLexemeData lexeme = lexemes[index];
            switch (parsingStack.top())
            {
                /* Non-Terminal Tokens */
            case E_BVH_TOKEN_NT_BOTTOM:
                // end sign
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_ENDSIGN)
                {
                    index++;
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_NT_FILE:
                // HIERARCHY
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_HIERARCHY)
                {
                    // <FILE> --> <HIERARCHY> <MOTION>
                    parsingStack.pop();
                    parsingStack.push(E_BVH_TOKEN_NT_MOTION);
                    parsingStack.push(E_BVH_TOKEN_NT_HIERARCHY);
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_NT_HIERARCHY:
                // HIERARCHY
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_HIERARCHY)
                {
                    // <HIERARCHY> --> HIERARCHY <ROOT_LIST>
                    parsingStack.pop();
                    parsingStack.push(E_BVH_TOKEN_NT_ROOT_LIST);
                    index++;
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_NT_ROOT_LIST:
                // ROOT
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_ROOT)
                {
                    // <ROOT_LIST> --> <ROOT> <ROOT_LIST_CONTINUE>
                    parsingStack.pop();
                    parsingStack.push(E_BVH_TOKEN_NT_ROOT_LIST_CONTINUE);
                    parsingStack.push(E_BVH_TOKEN_NT_ROOT);
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_NT_ROOT:
                // ROOT
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_ROOT)
                {
                    // <ROOT> --> ROOT identifier { <OFFSET> <CHANNELS> <JOINT_LIST> }
                    parsingStack.pop();
                    parsingStack.push(E_BVH_TOKEN_TKW_CLOSE_BRACKET);
                    parsingStack.push(E_BVH_TOKEN_NT_JOINT_LIST);
                    parsingStack.push(E_BVH_TOKEN_NT_CHANNELS);
                    parsingStack.push(E_BVH_TOKEN_NT_OFFSET);
                    parsingStack.push(E_BVH_TOKEN_TKW_OPEN_BRACKET);
                    parsingStack.push(E_BVH_TOKEN_T_IDENTIFIER);
                    index++;

                    bvhHierarchyNodeStack.push(BvhHierarchyNode(++currSkeletonIndex, -1));
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_NT_ROOT_LIST_CONTINUE:
                // ROOT
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_ROOT)
                {
                    // <ROOT_LIST_CONTINUE> --> <ROOT> <ROOT_LIST_CONTINUE>
                    parsingStack.push(E_BVH_TOKEN_NT_ROOT);
                }
                // MOTION or EndSign
                else if ((lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_MOTION) ||
                    (lexeme.LexemeType == E_BVH_LEXEME_TYPE_ENDSIGN))
                {
                    // <ROOT_LIST_CONTINUE> --> epsilon
                    parsingStack.pop();
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_NT_OFFSET:
                // OFFSET
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_OFFSET)
                {
                    // <OFFSET> --> OFFSET float_offset float_offset float_offset
                    parsingStack.pop();
                    parsingStack.push(E_BVH_TOKEN_T_FLOAT_OFFSET);
                    parsingStack.push(E_BVH_TOKEN_T_FLOAT_OFFSET);
                    parsingStack.push(E_BVH_TOKEN_T_FLOAT_OFFSET);
                    index++;

                    currOffsetFilledIndex = 0;
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_NT_CHANNELS:
                // CHANNELS
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_CHANNELS)
                {
                    // <CHANNELS> --> CHANNELS integer_channels <KEYWORD_POSITION_LIST>
                    parsingStack.pop();
                    parsingStack.push(E_BVH_TOKEN_NT_KEYWORD_POSITION_LIST);
                    parsingStack.push(E_BVH_TOKEN_T_INTEGER_CHANNELS);
                    index++;
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_NT_KEYWORD_POSITION_LIST:
                // Xposition, Yposition, Zposition, Xrotation, Yrotation, Zrotation
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD &&
                    lexeme.getKeywordValue() >= E_BVH_TOKEN_TKW_XPOSITION && lexeme.getKeywordValue() <= E_BVH_TOKEN_TKW_ZROTATION)
                {
                    // <KEYWORD_POSITION_LIST> --> <KEYWORD_POSITION> <KEYWORD_POSITION_LIST>
                    parsingStack.push(E_BVH_TOKEN_NT_KEYWORD_POSITION);
                }
                // JOINT
                else if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_JOINT)
                {
                    // <KEYWORD_POSITION_LIST> --> epsilon
                    parsingStack.pop();
                }
                // End
                else if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_END)
                {
                    // <KEYWORD_POSITION_LIST> --> epsilon
                    parsingStack.pop();
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_NT_KEYWORD_POSITION:
                // Xposition, Yposition, Zposition, Xrotation, Yrotation, Zrotation
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD &&
                    lexeme.getKeywordValue() >= E_BVH_TOKEN_TKW_XPOSITION && lexeme.getKeywordValue() <= E_BVH_TOKEN_TKW_ZROTATION)
                {
                    // <KEYWORD_POSITION> --> Xposition | Yposition | Zposition | Xrotation | Yrotation | Zrotation
                    parsingStack.pop();
                    index++;
                    // currently these keywords are simply ignored
                    // channel order is assumed to be always the same:
                    // for ROOT: Xposition Yposition Zposition Zrotation Xrotation Yrotation
                    // for JOINT: Zrotation Xrotation Yrotation
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_NT_JOINT_LIST:
                // JOINT
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_JOINT)
                {
                    // <JOINT_LIST> --> JOINT identifier { <OFFSET> <CHANNELS> <JOINT_LIST> } <JOINT_LIST_CONTINUE>
                    parsingStack.pop();
                    parsingStack.push(E_BVH_TOKEN_NT_JOINT_LIST_CONTINUE);
                    parsingStack.push(E_BVH_TOKEN_TKW_CLOSE_BRACKET);
                    parsingStack.push(E_BVH_TOKEN_NT_JOINT_LIST);
                    parsingStack.push(E_BVH_TOKEN_NT_CHANNELS);
                    parsingStack.push(E_BVH_TOKEN_NT_OFFSET);
                    parsingStack.push(E_BVH_TOKEN_TKW_OPEN_BRACKET);
                    parsingStack.push(E_BVH_TOKEN_T_IDENTIFIER);
                    index++;


                    bvhHierarchyNodeStack.push(BvhHierarchyNode(
                        currSkeletonIndex,
                        getParentIndex(bvhHierarchyNodeStack)));
                }
                // End
                else if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_END)
                {
                    // <JOINT_LIST> --> End Site { <OFFSET> }
                    parsingStack.pop();
                    parsingStack.push(E_BVH_TOKEN_TKW_CLOSE_BRACKET);
                    parsingStack.push(E_BVH_TOKEN_NT_OFFSET);
                    parsingStack.push(E_BVH_TOKEN_TKW_OPEN_BRACKET);
                    parsingStack.push(E_BVH_TOKEN_TKW_SITE);
                    index++;

                    bvhHierarchyNodeStack.push(BvhHierarchyNode(
                        currSkeletonIndex,
                        getParentIndex(bvhHierarchyNodeStack)));
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_NT_JOINT_LIST_CONTINUE:
                // JOINT
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_JOINT)
                {
                    // <JOINT_LIST> --> JOINT identifier { <OFFSET> <CHANNELS> <JOINT_LIST> } <JOINT_LIST_CONTINUE>
                    parsingStack.pop();
                    parsingStack.push(E_BVH_TOKEN_NT_JOINT_LIST_CONTINUE);
                    parsingStack.push(E_BVH_TOKEN_TKW_CLOSE_BRACKET);
                    parsingStack.push(E_BVH_TOKEN_NT_JOINT_LIST);
                    parsingStack.push(E_BVH_TOKEN_NT_CHANNELS);
                    parsingStack.push(E_BVH_TOKEN_NT_OFFSET);
                    parsingStack.push(E_BVH_TOKEN_TKW_OPEN_BRACKET);
                    parsingStack.push(E_BVH_TOKEN_T_IDENTIFIER);
                    index++;

                    bvhHierarchyNodeStack.push(BvhHierarchyNode(
                        currSkeletonIndex,
                        getParentIndex(bvhHierarchyNodeStack)));
                }
                // }
                else if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_SPECIALSIGN && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_CLOSE_BRACKET)
                {
                    // <JOINT_LIST_CONTINUE> --> epsilon
                    parsingStack.pop();
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_NT_MOTION:
                // MOTION
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_MOTION)
                {
                    // <MOTION> --> MOTION Frames : integer_frames Frame Time : float_frametime <MOTION_FLOAT_LIST>
                    parsingStack.pop();
                    parsingStack.push(E_BVH_TOKEN_NT_MOTION_FLOAT_LIST);
                    parsingStack.push(E_BVH_TOKEN_T_FLOAT_FRAMETIME);
                    parsingStack.push(E_BVH_TOKEN_TKW_COLON);
                    parsingStack.push(E_BVH_TOKEN_TKW_TIME);
                    parsingStack.push(E_BVH_TOKEN_TKW_FRAME);
                    parsingStack.push(E_BVH_TOKEN_T_INTEGER_FRAMES);
                    parsingStack.push(E_BVH_TOKEN_TKW_COLON);
                    parsingStack.push(E_BVH_TOKEN_TKW_FRAMES);
                    index++;

                    // calculate total number of channels in hierarchy
                    channelsNum = getNumberOfChannels(bvhHierarchyNodeList);
                }
                // EndSign
                else if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_ENDSIGN)
                {
                    // <MOTION> --> epsilon
                    parsingStack.pop();
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_NT_MOTION_FLOAT_LIST:
                // <MOTION_FLOAT_LIST> --> float_motiondata ... float_motiondata
                parsingStack.pop();

                motionDataSize = channelsNum * framesNum;
                for (int i = 0; i < motionDataSize; i++)
                {
                    parsingStack.push(E_BVH_TOKEN_T_FLOAT_MOTIONDATA);
                }

                if (motionData != NULL)
                {
                    delete [] motionData;
                    motionData = NULL;
                }

                if (motionDataSize > 0)
                {
                    motionData = new float[motionDataSize];
                }
                else
                {
                    throw new exception("Invalid motion data size.");
                }

                currMotionDataFilledIndex = 0;
                break;
                /* End Non-Terminal Tokens */
                
                /* Terminal Tokens */
            case E_BVH_TOKEN_TKW_HIERARCHY:
            case E_BVH_TOKEN_TKW_ROOT:
            case E_BVH_TOKEN_TKW_JOINT:
            case E_BVH_TOKEN_TKW_OFFSET:
            case E_BVH_TOKEN_TKW_CHANNELS:
            case E_BVH_TOKEN_TKW_XPOSITION:
            case E_BVH_TOKEN_TKW_YPOSITION:
            case E_BVH_TOKEN_TKW_ZPOSITION:
            case E_BVH_TOKEN_TKW_XROTATION:
            case E_BVH_TOKEN_TKW_YROTATION:
            case E_BVH_TOKEN_TKW_ZROTATION:
            case E_BVH_TOKEN_TKW_END:
            case E_BVH_TOKEN_TKW_MOTION:
                // these should not appear on stack
                // when applying productions they should automatically be removed
                // if this appears this is an error
                raiseException(lexeme.LineNumber);
                break;

            case E_BVH_TOKEN_TKW_SITE:
                // Site
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_SITE)
                {
                    parsingStack.pop();
                    index++;

                    bvhHierarchyNodeStack.top().NodeIndex = currNodeIndex++;
                    strncpy_s(bvhHierarchyNodeStack.top().NodeName, BvhReader::getEndSiteNodeName().c_str(), NODE_NAME_MAX);
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_TKW_FRAMES:
                // Frames
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_FRAMES)
                {
                    parsingStack.pop();
                    index++;
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_TKW_FRAME:
                // Frame
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_FRAME)
                {
                    parsingStack.pop();
                    index++;
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_TKW_TIME:
                // Time
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_KEYWORD && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_TIME)
                {
                    parsingStack.pop();
                    index++;
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_TKW_OPEN_BRACKET:
                // {
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_SPECIALSIGN && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_OPEN_BRACKET)
                {
                    parsingStack.pop();
                    index++;
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_TKW_CLOSE_BRACKET:
                // }
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_SPECIALSIGN && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_CLOSE_BRACKET)
                {
                    parsingStack.pop();
                    index++;

                    bvhHierarchyNodeList.push_back(bvhHierarchyNodeStack.top());
                    bvhHierarchyNodeStack.pop();
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_TKW_COLON:
                // :
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_SPECIALSIGN && lexeme.getKeywordValue() == E_BVH_TOKEN_TKW_COLON)
                {
                    parsingStack.pop();
                    index++;
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_T_IDENTIFIER:
                // identifier
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_IDENTIFIER)
                {
                    parsingStack.pop();
                    index++;

                    bvhHierarchyNodeStack.top().NodeIndex = currNodeIndex++;
                    strncpy_s(bvhHierarchyNodeStack.top().NodeName, lexeme.getIdentifierValue().c_str(), NODE_NAME_MAX);
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_T_INTEGER_CHANNELS:
                // integer
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_INTEGER)
                {
                    parsingStack.pop();
                    index++;

                    int nodeChannelCount = lexeme.getIntValue();
                    bvhHierarchyNodeStack.top().FirstChannelIndex = currFirstChannelIndex;
                    bvhHierarchyNodeStack.top().ChannelCount = nodeChannelCount;

                    currFirstChannelIndex += nodeChannelCount;
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_T_INTEGER_FRAMES:
                // integer
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_INTEGER)
                {
                    parsingStack.pop();
                    index++;

                    framesNum = lexeme.getIntValue();
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_T_FLOAT_OFFSET:
                // float
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_FLOAT)
                {
                    parsingStack.pop();
                    index++;

                    bvhHierarchyNodeStack.top().Offset[currOffsetFilledIndex] = lexeme.getFloatValue();
                    currOffsetFilledIndex++;
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_T_FLOAT_FRAMETIME:
                // float
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_FLOAT)
                {
                    parsingStack.pop();
                    index++;

                    frameLength = lexeme.getFloatValue();
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;

            case E_BVH_TOKEN_T_FLOAT_MOTIONDATA:
                // float
                if (lexeme.LexemeType == E_BVH_LEXEME_TYPE_FLOAT)
                {
                    parsingStack.pop();
                    index++;

                    if (motionData == NULL)
                    {
                        throw new exception("Motion data not initialized.");
                    }

                    motionData[currMotionDataFilledIndex] = lexeme.getFloatValue();

                    currMotionDataFilledIndex++;
                }
                else
                {
                    raiseException(lexeme.LineNumber);
                }
                break;
                /* End Terminal Tokens */

            default:
                raiseException(lexeme.LineNumber);
            } /* End switch */

            //string xxx;

            //for (unsigned long j=0; j<parsingStack.size(); ++j)
            //{
            //	file << (m_mapTokens.find(parsingStack[j]))->second << " ";
            //	xxx += (m_mapTokens.find(parsingStack[j]))->second;
            //	xxx += " ";
            //}

            //xxx.clear();

            //file << "\n";
        }

        bvhData->setBvhNodes(bvhHierarchyNodeList);
        bvhData->setMotionData(motionData);
        bvhData->setMotionDataStride(channelsNum);
        bvhData->setFrameCount(framesNum);
        bvhData->setFrameTime(frameLength);
    }

    int BvhParser::getNumberOfChannels(const vector<BvhHierarchyNode> &bvhHierarchyNodeList)
    {
        int nodeCount = (int)bvhHierarchyNodeList.size();
        int numChannels = 0;
        for (int i = 0; i < nodeCount; i++)
        {
            numChannels += bvhHierarchyNodeList[i].ChannelCount;
        }

        return numChannels;
    }

    int BvhParser::getParentIndex(const stack<BvhHierarchyNode> &bvhHierarchyNodeStack)
    {
        if (!bvhHierarchyNodeStack.empty())
        {
            return bvhHierarchyNodeStack.top().NodeIndex;
        }
        else
        {
            return 0;
        }
    }

    void BvhParser::raiseException(const int lineNum)
    {
        stringstream ss;
        ss << "Lexing error at line " << lineNum << ".";
        throw new exception(ss.str().c_str());
    }
}
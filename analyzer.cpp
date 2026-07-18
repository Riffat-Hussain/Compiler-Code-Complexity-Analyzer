#include "analyzer.h"

using namespace std;

namespace
{
    int ifCount = 0;
    int elseCount = 0;
    int elseIfCount = 0;
    int forCount = 0;
    int whileCount = 0;

    int statementCount = 0;
    int totalNodes = 0;

    int nestingDepth = 0;
    int maxNestingDepth = 0;

    int complexityScore = 1;

    void resetAnalyzer()
    {
        ifCount = elseCount = elseIfCount = forCount = whileCount = 0;
        statementCount = totalNodes = 0;
        nestingDepth = maxNestingDepth = 0;
        complexityScore = 1;
    }

    bool isStatementNode(NodeType t)
    {
        switch (t)
        {
            case ND_VAR_DECL:
            case ND_ASSIGN:
            case ND_INC_DEC:
            case ND_IF:
            case ND_WHILE:
            case ND_FOR:
            case ND_RETURN:
            case ND_EXPR_STMT:
                return true;
            default:
                return false;
        }
    }

    // Recursively walks the tree. `depth` is the current block-nesting
    // level, incremented every time we descend into a Block node so
    // maxNestingDepth reflects how deeply blocks are nested inside
    // one another (matches the "nesting weight" the proposal describes).
    void walk(ASTNode *node, int depth)
    {
        if (!node) return;

        totalNodes++;

        if (isStatementNode(node->type))
            statementCount++;

        switch (node->type)
        {
            case ND_BLOCK:
                depth++;
                if (depth > maxNestingDepth) maxNestingDepth = depth;
                break;

            case ND_IF:
                ifCount++;
                // children[2], if present, is either another If (else-if)
                // or a Block (plain else).
                if (node->children.size() == 3)
                {
                    if (node->children[2]->type == ND_IF)
                        elseIfCount++;
                    else
                        elseCount++;
                }
                break;

            case ND_FOR:
                forCount++;
                break;

            case ND_WHILE:
                whileCount++;
                break;

            default:
                break;
        }

        for (ASTNode *child : node->children)
            walk(child, depth);
    }

    void calculateComplexity()
    {
        // Complexity = 1 + (decision points) + (nesting weight)
        // as defined in the term project proposal.
        int decisionPoints = ifCount + elseIfCount + forCount + whileCount;
        int nestingWeight = maxNestingDepth > 1 ? (maxNestingDepth - 1) : 0;

        complexityScore = 1 + decisionPoints + nestingWeight;
    }
}

void analyzeCode(ASTNode *root)
{
    resetAnalyzer();
    walk(root, 0);
    calculateComplexity();
}

int getIfCount()          { return ifCount; }
int getElseCount()        { return elseCount; }
int getElseIfCount()      { return elseIfCount; }
int getForCount()         { return forCount; }
int getWhileCount()       { return whileCount; }
int getDecisionPoints()   { return ifCount + elseIfCount + forCount + whileCount; }
int getMaxNestingDepth()  { return maxNestingDepth; }
int getStatementCount()   { return statementCount; }
int getTotalNodes()       { return totalNodes; }
int getComplexityScore()  { return complexityScore; }

string getComplexityLevel()
{
    if (complexityScore <= 5) return "Low";
    if (complexityScore <= 15) return "Medium";
    return "High";
}

#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
#include "ast.h"

using namespace std;

// Walks the parse tree produced by parse() and computes structural
// metrics + a complexity score. Must be called before the getters below.
void analyzeCode(ASTNode *root);

int getIfCount();
int getElseCount();
int getElseIfCount();
int getForCount();
int getWhileCount();
int getDecisionPoints();
int getMaxNestingDepth();
int getStatementCount();
int getTotalNodes();

int getComplexityScore();
string getComplexityLevel();

#endif

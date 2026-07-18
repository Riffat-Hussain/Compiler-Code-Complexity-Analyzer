#include <iostream>
#include "report.h"
#include "analyzer.h"

using namespace std;

static void printHeader()
{
    cout << "\n=====================================\n";
    cout << "       FINAL COMPLEXITY REPORT       \n";
    cout << "=====================================\n";
}

static void printMetrics()
{
    cout << "\n--------- Code Statistics ---------\n";
    cout << "Total AST Nodes        : " << getTotalNodes() << "\n";
    cout << "Total Statements       : " << getStatementCount() << "\n";

    cout << "\n--------- Structural Metrics ---------\n";
    cout << "If Statements          : " << getIfCount() << "\n";
    cout << "Else-If Branches       : " << getElseIfCount() << "\n";
    cout << "Else Statements        : " << getElseCount() << "\n";
    cout << "For Loops              : " << getForCount() << "\n";
    cout << "While Loops            : " << getWhileCount() << "\n";
    cout << "Decision Points        : " << getDecisionPoints() << "\n";
    cout << "Maximum Nesting Depth  : " << getMaxNestingDepth() << "\n";

    // Deliberately NOT called "Cyclomatic Complexity": that term refers to
    // McCabe's specific E - N + 2P formula. This is a simpler weighted
    // score defined in the term project proposal, so it's labeled as such.
    cout << "\nComplexity Score       : " << getComplexityScore() << "\n";
    cout << "Risk Classification    : " << getComplexityLevel() << "\n";
}

static void printFooter()
{
    cout << "\n=====================================\n";
    cout << "   Project Executed Successfully!    \n";
    cout << "=====================================\n";
}

void generateReport()
{
    printHeader();
    cout << "\nLexical Analysis      : Successful\n";
    cout << "Syntax Analysis       : Successful (parse tree built)\n";
    cout << "Structural Analysis   : Successful\n";
    printMetrics();
    printFooter();
}

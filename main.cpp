#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"
#include "analyzer.h"
#include "report.h"
#include "ast.h"

using namespace std;

int main(int argc, char *argv[])
{
    string filename = (argc > 1) ? argv[1] : "input.txt";
    bool showTree = (argc > 2 && string(argv[2]) == "--tree");

    cout << "=====================================\n";
    cout << " Compiler-Based Code Complexity Analyzer\n";
    cout << " Source file: " << filename << "\n";
    cout << "=====================================\n";

    try
    {
        // Phase 1: Lexical Analysis
        lexicalAnalysis(filename);
        cout << "\nLexical Analysis: " << tokens.size() - 1
             << " tokens generated.\n";

        // Phase 2: Syntax Analysis — builds an actual parse tree
        ASTNode *root = parse();
        cout << "Syntax Analysis: parse tree built successfully.\n";

        if (showTree)
        {
            cout << "\n========== PARSE TREE ==========\n";
            printAST(root);
        }

        // Phase 3: Structural Analysis (walks the parse tree)
        analyzeCode(root);

        // Phase 4: Final Report
        generateReport();
    }
    catch (const LexError &e)
    {
        cerr << "\nLexical Error (line " << e.line << "): " << e.message << "\n";
        return 1;
    }
    catch (const ParseError &e)
    {
        cerr << "\nSyntax Error (line " << e.line << "): " << e.message << "\n";
        return 1;
    }

    return 0;
}

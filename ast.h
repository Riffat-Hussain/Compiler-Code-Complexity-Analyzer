#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

using namespace std;

enum NodeType
{
    ND_PROGRAM,
    ND_FUNC_DEF,
    ND_BLOCK,
    ND_VAR_DECL,
    ND_ASSIGN,
    ND_INC_DEC,
    ND_IF,
    ND_WHILE,
    ND_FOR,
    ND_RETURN,
    ND_EXPR_STMT,
    ND_BINARY,
    ND_UNARY,
    ND_LITERAL,
    ND_IDENTIFIER
};

// A single node in the parse tree. `value` holds whatever is relevant
// for that node type (operator symbol, identifier name, literal text...).
// `children` holds sub-nodes; their meaning depends on node type and is
// documented next to each parse function in parser.cpp.
struct ASTNode
{
    NodeType type;
    string value;
    int line;
    vector<ASTNode *> children;

    ASTNode(NodeType t, string v = "", int l = 0)
        : type(t), value(move(v)), line(l) {}
};

string nodeTypeName(NodeType t);

// Pretty-prints the tree with indentation so it can actually be shown
// as "the parse tree" during the demo, not just claimed in the report.
void printAST(ASTNode *node, int depth = 0);

#endif

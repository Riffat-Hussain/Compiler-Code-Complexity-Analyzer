#include <iostream>
#include "ast.h"

using namespace std;

string nodeTypeName(NodeType t)
{
    switch (t)
    {
        case ND_PROGRAM:    return "Program";
        case ND_FUNC_DEF:   return "FuncDef";
        case ND_BLOCK:      return "Block";
        case ND_VAR_DECL:   return "VarDecl";
        case ND_ASSIGN:     return "Assign";
        case ND_INC_DEC:    return "IncDec";
        case ND_IF:         return "If";
        case ND_WHILE:      return "While";
        case ND_FOR:        return "For";
        case ND_RETURN:     return "Return";
        case ND_EXPR_STMT:  return "ExprStmt";
        case ND_BINARY:     return "BinaryExpr";
        case ND_UNARY:      return "UnaryExpr";
        case ND_LITERAL:    return "Literal";
        case ND_IDENTIFIER: return "Identifier";
    }
    return "Unknown";
}

void printAST(ASTNode *node, int depth)
{
    if (!node) return;

    cout << string(depth * 2, ' ') << nodeTypeName(node->type);
    if (!node->value.empty())
        cout << " (" << node->value << ")";
    cout << endl;

    for (ASTNode *child : node->children)
        printAST(child, depth + 1);
}

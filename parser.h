#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

// Thrown when the token stream does not match the grammar.
// Carries the source line so the report can point at the problem.
struct ParseError
{
    string message;
    int line;
};

// Parses the global token stream (see lexer.h) and returns the root
// of the parse tree (a Program node). Throws ParseError on a syntax
// error instead of silently skipping the offending token.
ASTNode *parse();

#endif

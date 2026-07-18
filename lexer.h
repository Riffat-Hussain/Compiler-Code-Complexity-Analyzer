#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

using namespace std;

// One token produced by the lexer.
// type  : KEYWORD | IDENTIFIER | NUMBER | OPERATOR | DELIMITER | EOF_TOKEN
// value : the raw lexeme, e.g. "if", "x", "42", "==", "{"
// line  : source line the token started on (used for error messages)
struct Token
{
    string type;
    string value;
    int line;
};

// Global token stream produced by lexicalAnalysis().
extern vector<Token> tokens;

// Thrown when the lexer finds a character it cannot classify,
// or an unterminated string/char literal or block comment.
struct LexError
{
    string message;
    int line;
};

// Scans the given source file and fills the global `tokens` vector.
// Throws LexError on malformed input instead of silently dropping characters.
void lexicalAnalysis(const string &filename);

#endif
